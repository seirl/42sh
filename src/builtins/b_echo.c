#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell_private.h"
#include "macros.h"
#include "shopt.h"

struct echo_arg
{
    int n;
    int e;
    char **argv;
    int pos;
};
typedef struct echo_arg s_echo_arg;

static int check_arg(s_echo_arg *arg, char c)
{

    if (c == 'n')
        arg->n = 1;
    else if (c == 'e')
        arg->e = 1;
    else if (c == 'E')
        arg->e = 0;
    else
        return 0;
    return 1;
}

static s_echo_arg parse_argv(s_shell *shell, int argc, char *argv[])
{
    s_echo_arg arg;
    int i;
    arg.n = 0;
    arg.e = shopt_get(shell, "xpg_echo");
    arg.argv = argv;

    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            for (int j = 1; argv[i][j]; ++j)
            {
                if (check_arg(&arg, argv[i][j]) == 0)
                {
                    arg.pos = i;
                    return arg;
                }
            }
        }
        else
            break;
    }
    arg.pos = i;
    return arg;
}

static void do_echo(s_echo_arg arg, int argc)
{
    for (int i = arg.pos; i < argc; ++i)
        printf("%s ", arg.argv[i]);
    if (arg.n == 0)
        printf("\n");
}

int builtin_echo(s_shell *shell, int argc, char *argv[])
{
    s_echo_arg arg = parse_argv(shell, argc, argv);
    do_echo(arg, argc);
    return 0;
}
