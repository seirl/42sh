#include <stdlib.h>
#include "string_utils.h"

int string_atoi(s_string *n)
{
    return atoi(n->buf);
}

static void string_itoa_rec(unsigned long n, s_string *s)
{
    if (n == 0)
        return;
    string_itoa_rec(n / 10, s);
    string_putc(s, n % 10 + '0');
}

s_string *string_itoa(long n)
{
    s_string *s = string_create(0);
    if (n < 0)
    {
        n = -n;
        string_putc(s, '-');
    }
    string_itoa_rec(n, s);
    return s;
}
