#include "exec.h"
#include "xsyscall.h"

static s_pipe *pipe_init(void)
{
    s_pipe *pipe = smalloc(sizeof (s_pipe));
    pipe->next_proc = 0;
    pipe->nb_proc = 0;
    pipe->process = NULL;
    return pipe;
}

static void process_add(pid_t pid, s_pipe *pipe)
{
    ++pipe->nb_proc;
    pipe->process = srealloc(pipe->process, pipe->nb_proc * sizeof (pid_t));
    pipe->process[pipe->nb_proc - 1] = pid;
}

static pid_t proc_get_next(s_pipe *pipe)
{
    if (!pipe->nb_proc || pipe->next_proc >= pipe->nb_proc)
        return -1;
    pid_t process = pipe->process[pipe->next_proc];
    ++pipe->next_proc;
    return process;
}

static void pipe_child_job(int cmd_index,
                           int max_index,
                           int curr_pipe[2],
                           int old_pipe[2])
{
    if (cmd_index > 0)
        exec_pipe_setio(old_pipe, 1);
    if (cmd_index < max_index - 1)
        exec_pipe_setio(curr_pipe, 0);
}

static void close_pipe(int pipe[2])
{
    XCLOSE(pipe[0]);
    XCLOSE(pipe[1]);
}

static void pipe_parent_job(int index,
                            int max,
                            int (*old_pipe)[2],
                            int new_pipe[2])
{
    if (index > 0)
        close_pipe(*old_pipe);
    if (index < max - 1)
    {
        (*old_pipe)[0] = new_pipe[0];
        (*old_pipe)[1] = new_pipe[1];
    }
}

int exec_pipe(s_pipe *pipe_struct,
              s_ast_cmd **pipe_cmds,
              int len)
{
    int curr_pipe[2];
    int old_pipe[2];
    int unused = 0;
    for (int i = 0; i < len; ++i)
    {
        if (i < len - 1)
            unused += pipe(curr_pipe);
        pid_t pid = fork();
        if (pid == 0)
        {
            pipe_child_job(i, len, curr_pipe, old_pipe);
            exec_cmd_node(pipe_cmds[i]);
            if (g_shell.status < 0)
                fprintf(stderr, "Failed to execute command.\n");
            exit(1);
        }
        else if (pid > 0)
        {
            process_add(pid, pipe_struct);
            pipe_parent_job(i, len, &old_pipe, curr_pipe);
            if (i == len - 1)
                waitpid(pid, &g_shell.status, 0);
        }
    }
    return 0;
}

static void wait_pipe(pid_t main, s_pipe *pipe)
{
    int st = 0;
    pid_t proc = 0;

    if (main == 0)
    {
        waitpid((proc = proc_get_next(pipe)), &st, 0);
        g_shell.status = st;
    }
    proc = proc_get_next(pipe);
    while (proc >= 0)
    {
        kill(proc, SIGPIPE);
        waitpid(proc, &st, 0);
        proc = proc_get_next(pipe);
        g_shell.status = st;
    }
}

void exec_pipe_node(s_ast_pipeline *node)
{
    assert(node);
    int len = pipe_cmd_count(node);
    s_ast_cmd **pipe_cmds = pipe_cmd_array(node, len);

    if (len == 1)
        exec_cmd_node(pipe_cmds[0]);
    else if (len > 1)
    {
        s_pipe *pipe = pipe_init();
        int res = exec_pipe(pipe, pipe_cmds, len);
        if (res != 0)
        {
            fprintf(stderr, "Fail to exec pipe node");
            g_shell.status = -1;
        }
        wait_pipe(!res, pipe);
        sfree(pipe->process);
        sfree(pipe);
    }
    sfree(pipe_cmds);
}
