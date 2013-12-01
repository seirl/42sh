#include <stdlib.h>
#include <string.h>

#include "eval.h"
#include "ops.h"

static const s_op ops[] =
{
    { "+", 10, ASSOC_R, 1, op_plus },
    { "-", 10, ASSOC_R, 1, op_minus },
    { "~", 10, ASSOC_R, 1, op_not },
    { "!", 10, ASSOC_R, 1, op_bang },
    { "**", 9, ASSOC_R, 0, op_pow },
    { "/", 8, ASSOC_L, 0, op_div },
    { "%", 8, ASSOC_L, 0, op_mod },
    { "*", 8, ASSOC_L, 0, op_mul },
    { "+", 7, ASSOC_L, 0, op_add },
    { "-", 7, ASSOC_L, 0, op_sub },
    { "<<", 6, ASSOC_L, 0, op_lshift },
    { ">>", 6, ASSOC_L, 0, op_rshift },
    { "&", 5, ASSOC_L, 0, op_bitand },
    { "^", 4, ASSOC_L, 0, op_xor },
    { "|", 3, ASSOC_L, 0, op_bitor },
    { "&&", 2, ASSOC_L, 0, op_and },
    { "||", 1, ASSOC_L, 0, op_or },
    { "(", 0, ASSOC_L, 0, NULL },
    { ")", 0, ASSOC_L, 0, NULL },
};

const s_op *get_op(char *c, int unary)
{
    const s_op *current = NULL;
    for (unsigned i = 0; i < sizeof (ops) / sizeof (*ops); i++)
        if (!strncmp(ops[i].op, c, strlen(ops[i].op)))
            if (!current || (ops[i].unary == unary &&
                strlen(ops[i].op) >= strlen(current->op)))
                current = ops + i;
    return current;
}
