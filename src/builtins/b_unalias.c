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
    {
        'a', NULL, 0, 0, NEW_ARG
    },
};

static void usage(void)
{
    fprintf(stderr, "unalias: usage: unalias [-a] name [name ...]\n");
}

static void not_found(char *alias)
{
    fprintf(stderr, "unalias: %s: not found\n", alias);
}

static int remove_alias(s_shell *shell, s_opt *opt)
{
    int ret = 0;
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        s_string *key = string_create_from(opt_trailing_arg(opt, i));
        if (alias_unset(shell, key))
        {
            not_found(key->buf);
            ret = 1;
        }
        string_free(key);
    }
    return ret;
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
        ret = remove_alias(shell, opt);
    opt_free(opt);
    return ret;
}
