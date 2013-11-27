#define UNUSED_UNARY(Arg) Arg = Arg - 1 + 1

int op_plus(int a, int b)
{
    UNUSED_UNARY(b);
    return a;
}

int op_minus(int a, int b)
{
    UNUSED_UNARY(b);
    return -a;
}

int op_bang(int a, int b)
{
    UNUSED_UNARY(b);
    return !a;
}

int op_not(int a, int b)
{
    UNUSED_UNARY(b);
    return ~a;
}
