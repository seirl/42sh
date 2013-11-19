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

int exec_program(char **cmd_argv, s_ast_prefix *prefixes)
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
    return st;
}

void exec_cmd_word(s_ast_compound_word *word)
{
    int len = compound_word_len(word);
    handler callback = NULL;
    char **cmd_argv = compword_to_argv(word, len);

    s_ast_shell_cmd *func_body = funcs_get(cmd_argv[0]);
    if (func_body)
        exec_shell_cmd(func_body);
    else
    {
        if ((callback = builtin_handler(cmd_argv[0])) != NULL)
        {
            shell.status = callback(cmd_argv);
        }
        else
            shell.status = exec_program(cmd_argv, NULL);
    }
    for (int i = 0; i < len; ++i)
        sfree(cmd_argv[i]);
    sfree(cmd_argv);
}

void exec_simple_cmd(s_ast_simple_cmd *cmd)
{
    int len = element_list_len(cmd->elements);
    handler callback = NULL;
    exec_elements_redir(cmd->elements);
    char **cmd_argv = elements_to_argv(cmd->elements, len);
    s_ast_shell_cmd *func_body = funcs_get(cmd_argv[0]);

    if (func_body)
    {
        exec_prefixes(cmd->prefixes);
        exec_shell_cmd(func_body);
    }
    else
    {
        if ((callback = builtin_handler(cmd_argv[0])) != NULL)
        {
            exec_prefixes(cmd->prefixes);
            shell.status = callback(cmd_argv);
        }
        else
            shell.status = exec_program(cmd_argv, cmd->prefixes);
    }
}
