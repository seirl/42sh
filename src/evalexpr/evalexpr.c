#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ops.h"
#include "log.h"
#include "stack.h"
#include "shuntingyard.h"

static char *skip_blank(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}

static int cleanup(s_opstack *so, s_numstack *sn)
{
    while (so->size)
        rpn_eval(so, sn);
    if (sn->size > 1)
    {
        LOG(ERROR, "Too many remaining elements on the stack (%d instead of 1)"
                   ", missing operator", sn->size);
        return 1;
    }
    return 0;
}

long parse(s_opstack *so, s_numstack *sn, char *s)
{
    const s_op *op = NULL;
    const s_op *last_op = NULL;
    for (char *cs = s; *cs; cs = skip_blank(cs))
        if ((op = get_op(cs)))
        {
            if (last_op && (cs == s || strcmp(last_op->op, ")")))
            {
                if (!strcmp(op->op, "-"))
                    op = get_op("_");
                else if (!strcmp(op->op, "+"))
                    op = get_op("#");
            }
            shuntingyard(so, sn, op);
            last_op = op;
            cs += strlen(op->op);
        }
        else if (isdigit(*cs))
        {
            numstack_push(sn, strtol(cs, &cs, 0));
            last_op = NULL;
        }
        else
            return 0;
    cleanup(so, sn);
    return sn->nums[0];
}

long evalexpr(const char *s)
{
    char *cpy = strdup(s);
    s_opstack *so = opstack_init();
    s_numstack *sn = numstack_init();
    long ret = parse(so, sn, skip_blank(cpy));
    free(cpy);
    opstack_free(so);
    numstack_free(sn);
    return ret;
}
