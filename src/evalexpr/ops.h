#ifndef EVAL_OP_H
# define EVAL_OP_H

typedef enum assoc
{
    ASSOC_L,
    ASSOC_R,
} e_assoc;

typedef struct op
{
    char *op;
    int precedency;
    e_assoc associativity;
    int unary;
    long (*eval)(long, long);
} s_op;

const s_op *get_op(char *c, int unary);

#endif /* !EVAL_OP_H */
