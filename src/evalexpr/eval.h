#ifndef EVAL_H
# define EVAL_H

int op_plus(int a, int b);
int op_minus(int a, int b);
int op_bang(int a, int b);
int op_not(int a, int b);

int op_add(int a, int b);
int op_sub(int a, int b);
int op_mul(int a, int b);
int op_div(int a, int b);
int op_mod(int a, int b);

int op_pow(int a, int b);

int op_bitand(int a, int b);
int op_bitor(int a, int b);
int op_xor(int a, int b);
int op_and(int a, int b);
int op_or(int a, int b);

#endif /* !EVAL_H */
