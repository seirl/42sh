#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "getopt.h"
#include "shopt.h"
#include "macros.h"

static const s_param shopt_param[] =
{
    {
        's', NULL, 0, 0, NEW_ARG
    },
    {
        'u', NULL, 0, 0, NEW_ARG
    },
    {
        'q', NULL, 0, 0, NEW_ARG
    },
};

static void usage(void)
{
    fprintf(stdout, "shopt: usage: shopt [-qsu] [optname ...]\n");
}

static int foreach_opt(s_shell *shell, s_opt *opt, int value)
{
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        if (shopt_set(shell, opt_trailing_arg(opt, i), value) == 0)
        {
            fprintf(stderr, PROGNAME": shopt: %s: "
                    "invalid shell option name\n", opt_trailing_arg(opt, i));
            return 1;
        }
    }
    return 0;
}

static int print_shopt_argv(s_shell *shell, s_opt *opt, int print)
{
    int ret = 0;
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        char *name = opt_trailing_arg(opt, i);
        int shopt_value = shopt_get(shell, name);
        if (shopt_value == -1)
        {
            fprintf(stderr, PROGNAME": shopt: %s: "
                    "invalid shell option name\n", opt_trailing_arg(opt, i));
            return 1;
        }
        if (print)
            printf("%s\t%s\n", name, shopt_value == 1 ? "on" : "off");
        ret |= !shopt_value;
    }
    return ret;
}

int builtin_shopt(s_shell *shell, int argc, char *argv[])
{
    int ret = 0;
    int print = 1;
    s_opt *opt = opt_init(shopt_param, 3);
    if (opt_parse(argc, argv, opt))
    {
        usage();
        return 2;
    }
    if (opt_get(opt, "q", NULL))
        print = 0;
    if (opt->trailing_count == 0 && print)
        shopt_print(shell);
    else if (opt_get(opt, "s", NULL))
        ret = foreach_opt(shell, opt, 1);
    else if (opt_get(opt, "u", NULL))
        ret = foreach_opt(shell, opt, 0);
    else
        ret = print_shopt_argv(shell, opt, print);
    opt_free(opt);
    return ret;
}
