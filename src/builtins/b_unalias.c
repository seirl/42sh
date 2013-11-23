#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell.h"
#include "shell_private.h"
#include "getopt.h"
#include "alias.h"
#include "macros.h"

static const s_param shopt_param[] =
{
    { 'a', NULL, 0, 0, NEW_ARG },
};

static void usage(void)
{
    fprintf(stdout, "unalias: usage: unalias [-a] name [name ...]\n");
}

static void remove_alias(s_shell *shell, s_opt *opt)
{
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        s_string *key = string_create_from(opt_trailing_arg(opt, i));
        alias_unset(shell, key);
        string_free(key);
    }
}

int builtin_unalias(s_shell *shell, int argc, char *argv[])
{
    int ret = 0;
    s_opt *opt = opt_init(shopt_param, 3);
    if (opt_parse(argc, argv, opt))
    {
        usage();
        return 2;
    }
    if (opt_get(opt, "a", NULL))
        alias_clear(shell);
    else if (opt->trailing_count == 0)
        usage();
    else
        remove_alias(shell, opt);
    opt_free(opt);
    return ret;
}