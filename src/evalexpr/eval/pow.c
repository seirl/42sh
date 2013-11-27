int op_pow(int a, int b)
{
    return b ? ((b % 2) ? b : 1) * op_pow(a, b / 2) : 1;
}
