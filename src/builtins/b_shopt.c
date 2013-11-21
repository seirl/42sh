#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "getopt.h"
#include "shopt.h"
#include "macros.h"

static const s_param shopt_param[] =
{
    { 's', NULL, 0, 0, NEW_ARG },
    { 'u', NULL, 0, 0, NEW_ARG },
    { 'q', NULL, 0, 0, NEW_ARG },
};

static void usage(void)
{
    fprintf(stdout, "shopt: usage: shopt [-qsu] [optname ...]\n");
}

static int foreach_opt(s_opt *opt, int value)
{
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        if (shopt_set(opt_trailing_arg(opt, i), value) == 0)
        {
            fprintf(stderr, PROGNAME": shopt: %s: "
                    "invalid shell option name\n", opt_trailing_arg(opt, i));
            return 1;
        }
    }
    return 0;
}

int builtin_shopt(s_shell *shell, int argc, char *argv[])
{
    (void)shell;
    int ret = 0;
    s_opt *opt = opt_init(shopt_param , 3);
    if (opt_parse(argc, argv, opt))
    {
        usage();
        return 2;
    }
    if (opt->trailing_count == 0)
        shopt_print();
    else if (opt_get(opt, "s", NULL))
        ret = foreach_opt(opt, 1);
    else if (opt_get(opt, "u", NULL))
        ret = foreach_opt(opt, 0);
    opt_free(opt);
    return ret;
}
