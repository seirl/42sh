#include "string_utils.h"

void string_del_from_end(s_string *str, size_t n)
{
    while (n--)
    {
        if (str->len > 0)
        {
            str->len--;
            str->buf[str->len] = 0;
        }
    }
}
