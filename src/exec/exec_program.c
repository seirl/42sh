#include "exec.h"

void exec_argv(char **argv)
{
    assert(argv && argv[0]);
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

int exec_prog(char **cmd_argv,
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
        exec_prefixes(prefixes);
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

void exec_simple_cmd(s_ast_simple_cmd *cmd)
{
    int len = element_list_len(cmd->elements);
    handler callback = NULL;
    char **cmd_argv = elements_to_argv(cmd->elements, len);
    s_redir_context **contexts = exec_elements_redir(cmd->elements);
    if (!cmd_argv[0])
        return;
    s_ast_shell_cmd *func_body = funcs_get(cmd_argv[0]);

    if (func_body)
    {
        exec_prefixes(cmd->prefixes);
        exec_shell_cmd(func_body);
        restore_redir_contexts(contexts);
        sfree(cmd_argv);
    }
    else
    {
        if ((callback = builtin_handler(cmd_argv[0])) != NULL)
        {
            exec_prefixes(cmd->prefixes);
            shell.status = callback(cmd_argv);
        }
        else
            shell.status = exec_prog(cmd_argv, contexts, cmd->prefixes);
    }
}
