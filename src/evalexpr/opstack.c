#include <stdlib.h>

#include "log.h"
#include "ops.h"
#include "stack.h"

s_opstack *opstack_init(void)
{
    s_opstack *o = malloc(sizeof (s_opstack));
    o->size_max = 8;
    o->size = 0;
    o->ops = malloc(sizeof (const s_op *) * o->size_max);
    return o;
}

void opstack_push(s_opstack *s, const s_op *op)
{
    if (s->size >= s->size_max)
    {
        s->size_max *= 2;
        s->ops = realloc(s->ops, sizeof (const s_op *) * s->size_max);
    }
    s->ops[s->size++] = op;
}

const s_op *opstack_pop(s_opstack *s)
{
    if (!s->size)
    {
        LOG(ERROR, "%s", "Operator stack empty");
        return NULL;
    }
    return s->ops[--s->size];
}

void opstack_free(s_opstack *s)
{
    free(s->ops);
    free(s);
}
