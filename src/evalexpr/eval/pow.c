long op_pow(long a, long b)
{
    return b ? ((b % 2) ? a : 1) * op_pow(a * a, b / 2) : 1;
}
