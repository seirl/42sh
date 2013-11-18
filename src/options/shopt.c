#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shopt.h"

static s_shopt *shopt_get_ptr(size_t *size)
{
    static s_shopt shell_options[] =
    {
#define X(Name, Value) { Name, Value },
#include "shopt.def"
#undef X
    };
    *size = sizeof (shell_options) / sizeof (s_shopt);
    return shell_options;
}

//1: set, 0: does not exist
int shopt_set(const char *shopt, int value)
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
    {
        if (!strcmp(shopt, shell_options[i].name))
        {
            shell_options[i].value = value;
            return 1;
        }
    }
    return 0;
}

int shopt_get(const char *shopt)
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
        if (!strcmp(shopt, shell_options[i].name))
            return shell_options[i].value;
    return -1;
}

void shopt_print()
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
    {
        fprintf(stdout, "%s\t%s\n", shell_options[i].name,
                shell_options[i].value == 1 ? "on" : "off");
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
        if (!strcmp(Name, it->value.str))         \
            ok = 1;
#include "shopt.def"
#undef X
        if (ok == 0)
            RET_WITH(0, PROGNAME": %s: invalid shell option name\n",
                    it->value.str);
    }
    return 1;
}

int shopt_from_opt(s_opt *opt)
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    if (shopt_check(opt, "O") == 0)
        return 1;
    for (size_t i = 0 ; i < size; ++i)
    {
        int ret = opt_is_set(opt, "O", shell_options[i].name);
        if (ret != -1)
            shell_options[i].value = ret;
    }
    return 0;
}
