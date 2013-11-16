#include "exec.h"

struct ast_node *get_funct_node(char *func_name)
{
    assert(func_name);
    (void)func_name;
    return NULL; /* FIXME */
}

int is_function(char *func_name)
{
    assert(func_name);
    (void)func_name;
    return 0; /* FIXME */
}

int is_builtin(char *name)
{
    assert(name);
    (void)name;
    return 0; /* FIXME */
}

static void exec_cmd_prefix(char **pref)
{
    assert(pref && pref[0]);
    (void)pref;
    /* for (int i = 0; pref[i] != NULL; ++i) */
}

static void exec_func(char **func)
{
    assert(func && func[0]);
    (void)func;
    struct ast_node *func_node = get_funct_node(func[0]);
    exec_node(func_node);
}

static void exec_builtin(char *name, char **argv)
{
    assert(name && argv);
    (void)name;
    (void)argv;
    /* FIXME */
}

static void exec_bin(char **argv)
{
    if (argv == NULL)
    {
        fprintf(stderr, "Wrong binary string.\n");
        return;
    }
    execvp(argv[0], argv);
    /* FIXME: test errors on errno */
}

static int exec_cmd_bin(struct cmd_node *cmd)
{
    int st = 0;
    pid_t proc = fork();

    if (proc < 0)
    {
        fprintf(stderr, "failed fork\n");
        return -1;
    }
    if (proc == 0)
    {
        if (cmd->prefix != NULL)
            exec_cmd_prefix(cmd->prefix);
        exec_bin(cmd->argv);
        fprintf(stderr, "Execution flow corrupted.\n");
        assert(0);
        return 1;
    }
    waitpid(proc, &st, 0);
    return st;
}

void exec_cmd_node(struct cmd_node *cmd)
{
    assert(cmd);
    if (cmd->argv == NULL)
    {
        if (cmd->prefix == NULL)
            return;
        else
            exec_cmd_prefix(cmd->prefix);
    }
    if (is_function(cmd->argv[0]) > 0)
        exec_func(cmd->argv);
    else if (is_builtin(cmd->argv[0]) > 0)
        exec_builtin(cmd->argv[0], cmd->argv);
    else
        status = exec_cmd_bin(cmd);
}
