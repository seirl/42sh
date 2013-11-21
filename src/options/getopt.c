#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int string_assign(char *str, s_param *p)
{
    if (p->opt == 0)
        p->arg.str = NULL;
    else
    {
        if (str != NULL)
            p->arg.str = str;
        else
            return -1;
    }
    return 0;
}

static int add_arg(s_opt *opt, s_param p, char *o)
{
    if (string_assign(o, &p) == -1)
        return -1;
    p.arg.next = NULL;
    for (unsigned int i = 0; i < opt->count; ++i)
    {
        if (opt->param[i].short_name == p.short_name)
        {
            if (p.multi == 0)
                return p.opt;
            s_arg *new_arg = malloc(sizeof (s_arg));
            new_arg->str = p.arg.str;
            new_arg->set = p.arg.set;
            new_arg->next = opt->param[i].arg.next;
            opt->param[i].arg.next = new_arg;
            return p.opt;
        }
    }
    opt->param[opt->count++] = p;
    return p.opt;
}

static int handle_long_arg(s_opt *opt, char *arg, char *o)
{
    int ret;
    s_param p = { 0 };

    for (unsigned int i = 0; i < opt->valid_count; ++i)
    {
        if (opt->valid_param[i].long_name &&
           !strcmp(opt->valid_param[i].long_name, arg))
        {
            p.short_name = opt->valid_param[i].short_name;
            p.long_name = opt->valid_param[i].long_name;
            p.multi = opt->valid_param[i].multi;
            p.opt = opt->valid_param[i].opt;
            if ((ret = add_arg(opt, p, o)) == -1)
                RET_WITH(-2, PROGNAME": --%s: "
                        "option requieres an argument\n", p.long_name)
            return ret;
        }
    }
    RET_WITH(-1, PROGNAME": --%s: invalid option\n", arg);
}

static int handle_short_arg(s_opt *opt, char *arg, char *o, int set)
{
    s_param p = { 0 };
    int ret = -1;
    for (unsigned int i = 0; i < opt->valid_count; ++i)
    {
        if (opt->valid_param[i].short_name == arg[0])
        {
            p.short_name = opt->valid_param[i].short_name;
            p.long_name = opt->valid_param[i].long_name;
            p.multi = opt->valid_param[i].multi;
            p.opt = opt->valid_param[i].opt;
            p.arg.set = set;
            int ret = add_arg(opt, p, o);
            if (ret == -1)
                RET_WITH(-2, PROGNAME": -%c: "
                        "option requieres an argument\n", p.short_name)
            return ret;
        }
    }
    if (ret == -1)
        RET_WITH(-1, PROGNAME": -%s: invalid option\n", arg);
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
                ret = handle_long_arg(opt, &argv[i][2],
                                     i + 1 == argc ? NULL : argv[i + 1]);
            else
                ret = handle_short_arg(opt, &argv[i][1],
                                      i + 1 == argc ? NULL : argv[i + 1],
                                      argv[i][0] == '+');
            if (ret == -1 || ret == -2)
                return -ret;
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
            if (opt->param[i].long_name && !strcmp(opt->param[i].long_name, s))
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
        if (!strcmp(name, it->str))
            return it->set;
    }
    return -1;
}

int opt_get(s_opt *opt, const char *s, void *res)
{
    char **ptr_str = res;
    if (s == NULL)
        return 0;
    s_param *p = get_param(opt, s);
    if (p == NULL)
        return 0;
    if (p->opt)
        *ptr_str = p->arg.str;

    return 1;
}

char *opt_trailing_arg(s_opt *opt, unsigned int i)
{
    if (i >= opt->trailing_count)
        return NULL;
    return opt->trailing[i];
}
