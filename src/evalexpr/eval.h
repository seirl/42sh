#ifndef EVAL_H
# define EVAL_H

long op_plus(long a, long b);
long op_minus(long a, long b);
long op_bang(long a, long b);
long op_not(long a, long b);

long op_add(long a, long b);
long op_sub(long a, long b);
long op_mul(long a, long b);
long op_div(long a, long b);
long op_mod(long a, long b);

long op_pow(long a, long b);

long op_bitand(long a, long b);
long op_bitor(long a, long b);
long op_xor(long a, long b);
long op_and(long a, long b);
long op_or(long a, long b);

long op_rshift(long a, long b);
long op_lshift(long a, long b);

#endif /* !EVAL_H */
