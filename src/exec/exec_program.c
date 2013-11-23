#include "exec.h"

#include "shell_private.h"
#include "smalloc.h"
#include "functions.h"

void free_current_cmd(char **argv)
{
    int i = 0;
    if (argv)
    {
        while (argv[i])
        {
            sfree(argv[i]);
            i += 1;
        }
        sfree(argv);
    }
}

void exec_argv(char **argv)
{
    assert(argv && argv[0]);
    if (!strcmp(argv[0], "exit"))
    {
        free_current_cmd(argv);
        char *new_argv[] =
        {
            "exit\0",
            NULL
        };
        execvp(new_argv[0], new_argv);
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
              s_redir_context **contexts,
              s_ast_prefix *prefixes)
{
    pid_t pid;
    int st;

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
        fprintf(stderr, "Execution flow corrupted.\n");
        assert(0);
        return 1;
    }
    waitpid(pid, &st, 0);
    restore_redir_contexts(contexts);
    for (int i = 0; cmd_argv[i]; ++i)
        sfree(cmd_argv[i]);
    sfree(cmd_argv);
    return st;
}

void restore_redir_contexts(s_redir_context **contexts)
{
    if (!contexts)
        return;
    for (int i = 0; contexts[i]; ++i)
        restore_redir_context(contexts[i]);
    sfree(contexts);
}

void exec_simple_cmd(s_shell *shell, s_ast_simple_cmd *cmd)
{
    int len = element_list_len(cmd->elements);
    f_handler callback;
    char **cmd_argv = elements_to_argv(cmd->elements, len);
    s_redir_context **contexts = exec_elements_redir(shell, cmd->elements);
    s_ast_shell_cmd *func_body;

    if (!cmd_argv[0])
        exec_prefixes(shell, cmd->prefixes);
    else if ((func_body = functions_get(shell, cmd_argv[0])))
    {
        exec_prefixes(shell, cmd->prefixes);
        exec_shell_cmd(shell, func_body);
        restore_redir_contexts(contexts);
        sfree(cmd_argv);
    }
    else if ((callback = builtins_find(shell, cmd_argv[0])))
    {
        exec_prefixes(shell, cmd->prefixes);
        shell->status = callback(shell, len, cmd_argv);
        sfree(cmd_argv);
    }
    else
        shell->status = exec_prog(shell, cmd_argv, contexts, cmd->prefixes);
}
