#include "exec.h"

#include "shell_private.h"
#include "smalloc.h"
#include "functions.h"

static void exec_exit(void)
{
    char *new_argv[2];
    new_argv[0] = "exit";
    new_argv[1] = NULL;
    execvp(new_argv[0], new_argv);
}

static void exec_argv_free(char **cmd_argv)
{
    if (!cmd_argv)
        return;

    for (int i = 0; cmd_argv[i]; ++i)
        sfree(cmd_argv[i]);
    sfree(cmd_argv);
}

void exec_argv(char **argv)
{
    assert(argv && argv[0]);
    if (!strcmp(argv[0], "exit"))
    {
        exec_argv_free(argv);
        exec_exit();
    }
    execvp(argv[0], argv);
    if (errno == ENOENT)
    {
        if (strchr(argv[0], '/') == NULL)
            fprintf(stderr, "%s: command not found.\n", argv[0]);
        else
            fprintf(stderr, "%s: no such file or directory.\n", argv[0]);
    }
    exit((errno == ENOENT) ? 127 : 126);
}

int exec_prog(s_shell *shell,
              char **cmd_argv,
              s_ast_prefix *prefixes)
{
    pid_t pid;
    int st = 0;

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, "Cannot fork.\n");
        return -1;
    }
    if (pid == 0)
    {
        exec_prefixes(shell, prefixes);
        shell->curr_argv = cmd_argv;
        exec_argv(cmd_argv);
        assert(0 && "Execution flow corrupted.");
    }
    waitpid(pid, &st, 0);
    return WEXITSTATUS(st); // TODO check for other kind of exit
}

void restore_redir_contexts(s_redir_context **contexts)
{
    assert(contexts);
    for (int i = 0; contexts[i]; ++i)
        restore_redir_context(contexts[i]);
    sfree(contexts);
}

void exec_simple_cmd(s_shell *shell, s_ast_simple_cmd *cmd)
{
    int len = element_list_len(cmd->elements);
    char **cmd_argv = elements_to_argv(shell, cmd->elements, len);
    s_redir_context **contexts = exec_elements_redir(shell, cmd->elements);
    s_ast_shell_cmd *func_body;
    f_handler callback;
    int ret = 0;

    if ((ret = exec_prefixes(shell, cmd->prefixes)) != 0 || !cmd_argv
        || !cmd_argv[0])
    {
        shell_status_set(shell, ret);
        return; // TODO cleanup
    }

    if ((func_body = functions_get(shell, cmd_argv[0])))
        exec_shell_cmd(shell, func_body); // TODO return int status
    else if ((callback = builtins_find(shell, cmd_argv[0])))
        ret = callback(shell, len, cmd_argv);
    else
        ret = exec_prog(shell, cmd_argv, cmd->prefixes);

    exec_argv_free(cmd_argv);
    restore_redir_contexts(contexts);
    shell_status_set(shell, ret);
}
