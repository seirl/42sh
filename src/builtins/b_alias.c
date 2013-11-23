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
    { 'p', NULL, 0, 0, NEW_ARG },
};

static void usage(void)
{
    fprintf(stdout, "alias: usage: alias [-p] [name[=value] ... ]\n");
}

static int is_assignment(char *arg)
{
    for (int i = 0; arg[i]; ++i)
        if (arg[i] == '=')
            return 1;
    return 0;
}

static void split_assignment(char *arg, s_string *key, s_string *value)
{
    int side = 0;
    for (int i = 0; arg[i]; ++i)
    {
        if (arg[i] == '=' && side == 0)
        {
            side = 1;
            continue;
        }
        if (side == 0)
            string_putc(key, arg[i]);
        else
            string_putc(value, arg[i]);
    }
}

static int add_alias(s_shell *shell, s_opt *opt)
{
    for (unsigned int i = 0; i < opt->trailing_count; ++i)
    {
        char *arg = (opt_trailing_arg(opt, i));
        if (is_assignment(arg))
        {
            s_string *key = string_create(0);
            s_string *value = string_create(0);
            split_assignment(arg, key, value);
            alias_set(shell, key, value);
        }
        else
        {
            s_string *str_arg = string_create_from(arg);
            s_string *alias = alias_get(shell, str_arg);
            string_free(str_arg);
            if (alias)
                printf("%s='%s'\n", arg, alias->buf);
            else
                fprintf(stderr, PROGNAME": alias: %s: not found\n", arg);
        }
    }
    return 0;
}

int builtin_alias(s_shell *shell, int argc, char *argv[])
{
    int ret = 0;
    s_opt *opt = opt_init(shopt_param, 3);
    if (opt_parse(argc, argv, opt))
    {
        usage();
        return 2;
    }
    if (opt->trailing_count == 0)
        alias_print(shell);
    else if (opt_get(opt, "p", NULL))
        alias_print(shell);
    else
        add_alias(shell, opt);
    opt_free(opt);
    return ret;
}
