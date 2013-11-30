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

static const char escape_table[][2] =
{
    "a\n",
    "b\b",
    "e\27",
    "E\27",
    "f\f",
    "n\n",
    "r\r",
    "t\t",
    "v\v",
    "\\\\"
};

static int print_char(s_echo_arg arg, char *str, int j)
{
    int escaped = 0;
    if (str[j] == '\\')
        escaped = 1;
    if (str[j + escaped] != 0)
    {
        if (arg.e && escaped)
        {
            for (unsigned i = 0; i < sizeof (escape_table) / sizeof (char *);
                ++i)
            {
                if (escape_table[i][0] == str[j + escaped])
                {
                    putc(escape_table[i][1], stdout);
                    return j + escaped;
                }
            }
        }
        putc(str[j], stdout);
    }
    return j;
}

static void do_echo(s_echo_arg arg, int argc)
{
    for (int i = arg.pos; i < argc; ++i)
    {
        for (int j = 0; arg.argv[i][j]; ++j)
            j = print_char(arg, arg.argv[i], j);
        if (i + 1 != argc)
            putc(' ', stdout);
    }
    if (arg.n == 0)
        putc('\n', stdout);
}

int builtin_echo(s_shell *shell, int argc, char *argv[])
{
    s_echo_arg arg = parse_argv(shell, argc, argv);
    do_echo(arg, argc);
    fflush(stdout);
    return 0;
}
