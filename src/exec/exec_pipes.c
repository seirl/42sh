#include "exec.h"

static s_pipe *init_pipe(void)
{
    s_pipe *pipe = smalloc(sizeof (s_pipe));
    pipe->next_proc = 0;
    pipe->nb_proc = 0;
    pipe->process = NULL;
    return pipe;
}

static void add_process(pid_t pid, s_pipe *pipe)
{
    ++pipe->nb_proc;
    pipe->process = srealloc(pipe->process, pipe->nb_proc * sizeof (pid_t));
    pipe->process[pipe->nb_proc - 1] = pid;
}

static pid_t get_next_proc(s_pipe *pipe)
{
    if (!pipe->nb_proc || pipe->next_proc >= pipe->nb_proc)
        return -1;
    pid_t process = pipe->process[pipe->next_proc];
    ++pipe->next_proc;
    return process;
}

static pid_t exec_pipe_side_cmd(int fd_pipe[2],
                         struct ast_node *side_node,
                         s_pipe *pipe,
                         e_side side)
{
    int input = (side == RIGHT) ? 0 : 1;
    int output = (side == RIGHT) ? 1 : 0;
    pid_t pid = fork();

    if (pid < 0)
        return pid;
    if (pid == 0)
    {
        if (side == RIGHT)
            dup2(fd_pipe[input], STDIN_FILENO);
        else
            dup2(fd_pipe[input], STDOUT_FILENO);
        close(fd_pipe[output]);
        if (side_node->type == CMD)
            exec_cmd_node(&side_node->next.cmd_n);
        else
            exec_node(side_node);
        exit(status);
    }
    else
    {
        close(fd_pipe[input]);
        add_process(pid, pipe);
    }
    return pid;
}

static int exec_pipe_cmd(s_pipe *p, struct binary_node *node)
{
    int fd_pipe[2];
    if (pipe2(fd_pipe, 0) < 0)
        return 0; /* pipe failed */

    /* execute right node */
    pid_t pid = exec_pipe_side_cmd(fd_pipe, node->right, p, RIGHT);
    if (pid < 0)
        return 0; /* fork failed */

    if (node->left->type == PIPE)
    {
        int tmp = dup(STDOUT_FILENO);
        dup2(fd_pipe[1], STDOUT_FILENO);
        close(fd_pipe[1]);
        int res = exec_pipe_cmd(p, &node->left->next.pipe_n);
        dup2(tmp, STDOUT_FILENO);
        close(tmp);
        return res;
    }

    /* else execute left node */
    int res = exec_pipe_side_cmd(fd_pipe, node->left, p, LEFT);
    if (res < 0) /* fork failed */
        return 0;
    return 1;
}

static void wait_pipe(pid_t main, s_pipe *pipe)
{
    int st = 0;
    pid_t proc = 0;

    if (main == 0)
    {
        waitpid((proc = get_next_proc(pipe)), &st, 0);
        status = st;
    }
    proc = get_next_proc(pipe);
    while (proc >= 0)
    {
        kill(proc, SIGPIPE);
        waitpid(proc, &st, 0);
        proc = get_next_proc(pipe);
    }
}

void exec_pipe_node(struct binary_node *node)
{
    assert(node);
    s_pipe *pipe = init_pipe();
    int res = exec_pipe_cmd(pipe, node);
    if (res == 0)
    {
        fprintf(stderr, "Fail to exec pipe node");
        status = -1;
    }
    wait_pipe(!res, pipe);
    sfree(pipe);
}
