#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "ops.h"

static const s_op ops[] =
{
    { "#",  9, ASSOC_R, 1, op_plus },
    { "_",  9, ASSOC_R, 1, op_minus },
    { "**", 8, ASSOC_R, 0, op_pow },
    { "/",  7, ASSOC_L, 0, op_div },
    { "%",  7, ASSOC_L, 0, op_mod },
    { "*",  7, ASSOC_L, 0, op_mul },
    { "+",  6, ASSOC_L, 0, op_add },
    { "-",  6, ASSOC_L, 0, op_sub },
    { "~",  6, ASSOC_R, 1, op_not },
    { "!",  6, ASSOC_R, 1, op_bang },
    { "&&", 2, ASSOC_L, 0, op_and },
    { "||", 1, ASSOC_L, 0, op_or },
    { "&",  5, ASSOC_L, 0, op_bitand },
    { "^",  4, ASSOC_L, 0, op_xor },
    { "|",  3, ASSOC_L, 0, op_bitor },
    { "(",  0, ASSOC_L, 0, NULL },
    { ")",  0, ASSOC_L, 0, NULL },
};

const s_op *get_op(char *c)
{
    for (unsigned i = 0; i < sizeof (ops) / sizeof (*ops); i++)
        if (!strncmp(ops[i].op, c, strlen(ops[i].op)))
            return ops + i;
    return NULL;
}
