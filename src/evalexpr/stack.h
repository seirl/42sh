#ifndef EVAL_STACK_H
# define EVAL_STACK_H

# include <unistd.h>
# include "ops.h"

typedef struct opstack
{
    const s_op **ops;
    size_t size;
    size_t size_max;
} s_opstack;

s_opstack *opstack_init(void);
void opstack_push(s_opstack *s, const s_op *op);
const s_op *opstack_pop(s_opstack *s);
void opstack_free(s_opstack *s);

typedef struct numstack
{
    int *nums;
    size_t size;
    size_t size_max;
} s_numstack;

s_numstack *numstack_init(void);
void numstack_push(s_numstack *s, int num);
int numstack_pop(s_numstack *s);
void numstack_free(s_numstack *s);

#endif /* !EVAL_STACK_H */
