#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shopt.h"
#include "shell_private.h"
#include "env.h"

static void shopt_update_env(s_shell *shell)
{
    s_string *shell_opts = string_create(64);
    int first = 0;
    for (int i = 0; i < SHOPT_COUNT; ++i)
    {
        if (shell->shopt[i].value == 1)
        {
            if (first == 0)
                first = 1;
            else
                string_putc(shell_opts, ':');
            string_puts(shell_opts, shell->shopt[i].name);
        }
    }
    env_set(shell, shell_opts->buf, "SHELLOPTS");
    string_free(shell_opts);
}

void shopt_create(s_shell *shell)
{
    int i = 0;
#define X(Name, Value) \
    shell->shopt[i].name = Name; \
    shell->shopt[i].value = Value; \
    ++i;
#include "shopt.def"
#undef X
    shopt_update_env(shell);
}

//1: set, 0: does not exist
int shopt_set(s_shell *shell, const char *shopt, int value)
{
    for (size_t i = 0; i < SHOPT_COUNT; ++i)
    {
        if (!strcmp(shopt, shell->shopt[i].name))
        {
            shell->shopt[i].value = value;
            shopt_update_env(shell);
            return 1;
        }
    }
    return 0;
}

int shopt_get(s_shell *shell, const char *shopt)
{
    for (size_t i = 0; i < SHOPT_COUNT; ++i)
        if (!strcmp(shopt, shell->shopt[i].name))
            return shell->shopt[i].value;
    return -1;
}

void shopt_print(s_shell *shell)
{
    for (size_t i = 0; i < SHOPT_COUNT; ++i)
    {
        fprintf(stdout, "%s\t%s\n", shell->shopt[i].name,
                shell->shopt[i].value == 1 ? "on" : "off");
    }
}

static s_param *get_param(s_opt *opt, const char *s)
{
    if (s[0] && !s[1])
    {
        for (unsigned int i = 0; i < opt->count; ++i)
            if (opt->param[i].short_name == s[0])
                return &opt->param[i];
    }
    else
    {
        for (unsigned int i = 0; i < opt->count; ++i)
            if (!strcmp(opt->param[i].long_name, s))
                return &opt->param[i];
    }
    return NULL;
}

static int shopt_check(s_opt *opt, const char *arg)
{
    s_param *p = get_param(opt, arg);
    if (p == NULL)
        return 1;
    for (s_arg *it = &(p->arg); it; it = it->next)
    {
        int ok = 0;
#define X(Name, Value)                            \
        if (!strcmp(Name, it->str))               \
            ok = 1;
#include "shopt.def"
#undef X
        if (ok == 0)
            RET_WITH(0, PROGNAME": %s: invalid shell option name\n",
                    it->str);
    }
    return 1;
}

int shopt_from_opt(s_shell *shell, s_opt *opt)
{
    if (shopt_check(opt, "O") == 0)
        return 1;
    for (size_t i = 0; i < SHOPT_COUNT; ++i)
    {
        int ret = opt_is_set(opt, "O", shell->shopt[i].name);
        if (ret != -1)
            shell->shopt[i].value = ret;
    }
    shopt_update_env(shell);
    return 0;
}
