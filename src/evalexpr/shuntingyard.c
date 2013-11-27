#include <string.h>

#include "log.h"
#include "ops.h"
#include "stack.h"

void rpn_eval(s_opstack *so, s_numstack *sn)
{
    const s_op *popped = opstack_pop(so);
    int operand_1 = (popped->unary) ? 0 : numstack_pop(sn);
    int operand_2 = numstack_pop(sn);
    numstack_push(sn, popped->eval(operand_2, operand_1));
}

static int handle_parenthesis(s_opstack *so, s_numstack *sn, const s_op *o)
{
    const s_op *popped;
    if (!strcmp(o->op, "("))
        opstack_push(so, o);
    else if (!strcmp(o->op, ")"))
    {
        while (so->size > 0 && strcmp(so->ops[so->size - 1]->op, "("))
            rpn_eval(so, sn);
        if (!(popped = opstack_pop(so)) || strcmp(popped->op, "("))
            LOG(ERROR, "%s", "No matching '('");
    }
    else
        return 0;
    return 1;
}

void shuntingyard(s_opstack *so, s_numstack *sn, const s_op *o)
{
    if (handle_parenthesis(so, sn, o))
        return;
    if (o->associativity == ASSOC_R)
        while (so->size && o->precedency < so->ops[so->size - 1]->precedency)
            rpn_eval(so, sn);
    else
        while (so->size && o->precedency <= so->ops[so->size - 1]->precedency)
            rpn_eval(so, sn);
    opstack_push(so, o);
}
