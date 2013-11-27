#include <string.h>

#include "log.h"
#include "ops.h"
#include "stack.h"

static void popeval(s_opstack *so, s_numstack *sn)
{
    const s_op *popped = opstack_pop(so);
    int operand_1 = numstack_pop(sn);
    int operand_2 = (popped->unary) ? 0 : numstack_pop(sn);
    numstack_push(sn, popped->eval(operand_2, operand_1));
}

static int handle_parenthesis(s_opstack *so, s_numstack *sn, const s_op *o)
{
    const s_op *popped;
    if (!strcmp(o->op, "("))
        opstack_push(so, o);
    else if (!strcmp(o->op, ")"))
    {
        while (so->size > 0 && !strcmp(so->ops[so->size - 1]->op, "("))
            popeval(so, sn);
        if (!(popped = opstack_pop(so)) || strcmp(popped->op, "("))
            LOG(ERROR, "%s", "No matching '('");
    }
    else
        return 1;
    return 0;
}

void shuntingyard(s_opstack *so, s_numstack *sn, const s_op *o)
{
    if (handle_parenthesis(so, sn, o))
        return;
    if (o->associativity == ASSOC_R)
        while (so->size && o->precedency < so->ops[so->size - 1]->precedency)
            popeval(so, sn);
    else
        while (so->size && o->precedency <= so->ops[so->size - 1]->precedency)
            popeval(so, sn);
    opstack_push(so, o);
}
