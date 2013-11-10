#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int add_arg(s_opt *opt, s_param p, e_type t, char *o)
{
    if (t != e_void && o == NULL)
        return -1;
    if (t == e_void)
        p.arg.value.str = NULL;
    else if (t == e_str)
        p.arg.value.str = o;
    else if (t == e_int)
        p.arg.value.n = atoi(o);
    p.arg.next = NULL;
    for (unsigned int i = 0; i < opt->count; ++i)
    {
        if (opt->param[i].short_name == p.short_name)
        {
            if (p.multi == 0)
                return t != e_void;
            s_arg *new_arg = malloc(sizeof (s_arg));
            new_arg->value = p.arg.value;
            new_arg->set = p.arg.set;
            new_arg->next = opt->param[i].arg.next;
            opt->param[i].arg.next = new_arg;
            return t != e_void;
        }
    }
    opt->param[opt->count++] = p;
    return t != e_void;
}

static int handle_long_arg(s_opt *opt, char *arg, char *o)
{
    int ret;
    s_param p;
#define X(S, L, T, M)                                                       \
    if (strcmp("NULL", #L) && !strcmp(arg, #L))                             \
    {                                                                       \
        p.short_name = *#S;                                                 \
        p.long_name = #L;                                                   \
        p.type = e_##T;                                                     \
        p.multi = M;                                                        \
        if ((ret = add_arg(opt, p, e_##T, o)) == -1)                        \
        RET_WITH(-1, "--%s requiere an argument\n", #L)                     \
        return ret;                                                         \
    }
#include "arg.def"
#undef X
    return 0;
}

static int handle_short_arg(s_opt *opt, char *arg, char *o, int set)
{
    s_param p;
    int ret = 0;
    for (int i = 0; arg[i]; ++i)
    {
#define X(S, L, T, M)                                                       \
        if (*#S != '/' && *#S == arg[i])                                    \
        {                                                                   \
            p.short_name = *#S;                                             \
            p.long_name = #L;                                               \
            p.type = e_##T;                                                 \
            p.multi = M;                                                    \
            p.arg.set = set;                                                \
            ret = add_arg(opt, p, e_##T, arg[i + 1] ? NULL : o);            \
            if (ret == -1)                                                  \
            RET_WITH(-1, "-%c requiere an argument\n", *#S)                 \
        }
#include "arg.def"
#undef X
    }
    return ret;
}

static void handle_trailing_arg(s_opt *opt, char **arg)
{
    if (opt->trailing_count == 0)
        opt->trailing = arg;
    opt->trailing_count++;
}

int opt_parse(int argc, char *argv[], s_opt *opt)
{
    int ret;
    opt->count = 0;
    opt->trailing_count = 0;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-' || argv[i][0] == '+')
        {
            if (opt->trailing_count)
                RET_WITH(1, "Trailing argv at position %d (%s)\n",
                        i - 1, opt->trailing[0]);
            if (argv[i][1] == '-')
                ret = handle_long_arg(opt, &argv[i][2], argv[i + 1]);
            else
                ret = handle_short_arg(opt, &argv[i][1], argv[i + 1],
                        argv[i][0] == '+');
            if (ret == -1)
                return 1;
            i += ret;
        }
        else
            handle_trailing_arg(opt, &(argv[i]));
    }
    return 0;
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

int opt_is_set(s_opt *opt, const char *arg, const char *name)
{
    s_param *p = get_param(opt, arg);
    if (p == NULL)
        return 0;
    for (s_arg *it = &(p->arg); it; it = it->next)
    {
        if (!strcmp(name, it->value.str))
            return it->set;
    }
    return -1;
}

int opt_get(s_opt *opt, const char *s, void *res)
{
    int *ptr_int = res;
    char **ptr_str = res;
    if (s == NULL)
        return 0;
    s_param *p = get_param(opt, s);
    if (p == NULL)
        return 0;
    if (p->type == e_int)
        *ptr_int = p->arg.value.n;
    if (p->type == e_str)
        *ptr_str = p->arg.value.str;

    return 1;
}

char *opt_trailing_arg(s_opt *opt, unsigned int i)
{
    if (i >= opt->trailing_count)
        return NULL;
    return opt->trailing[i];
}

void opt_free(s_opt *opt)
{
    for (unsigned int i = 0; i < opt->count; ++i)
    {
        if (opt->param[i].arg.next)
        {
            s_arg *it = opt->param[i].arg.next;
            s_arg *tmp;
            while (it)
            {
                tmp = it->next;
                free(it);
                it = tmp;
            }
        }
    }
}
