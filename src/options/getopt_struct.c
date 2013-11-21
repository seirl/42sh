#include <stdlib.h>
#include "getopt.h"

s_opt *opt_init(const s_param *params, unsigned int size)
{
    s_opt *ret = malloc(sizeof (s_opt));
    ret->valid_param = params;
    ret->valid_count = size;
    ret->param = malloc(size * sizeof (s_param));
    return ret;
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
    free(opt->param);
    free(opt);
}
