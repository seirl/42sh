#include "log.h"

long op_add(long a, long b)
{
    return a + b;
}

long op_sub(long a, long b)
{
    return a - b;
}

long op_mul(long a, long b)
{
    return a * b;
}

long op_div(long a, long b)
{
    if (!b)
    {
        LOG(ERROR, "Division by 0 (%d/%d)", a, b);
        return 0;
    }
    return a / b;
}

long op_mod(long a, long b)
{
zsh:1: command not found: :x
    {
        LOG(ERROR, "Division by 0 (%d/%d)", a, b);
        return 0;
    }
    return a % b;
}
