#define UNUSED_UNARY(Arg) Arg = Arg - 1 + 1

long op_plus(long a, long b)
{
    UNUSED_UNARY(b);
    return a;
}

long op_minus(long a, long b)
{
    UNUSED_UNARY(b);
    return -a;
}

long op_bang(long a, long b)
{
    UNUSED_UNARY(b);
    return !a;
}

long op_not(long a, long b)
{
    UNUSED_UNARY(b);
    return ~a;
}
