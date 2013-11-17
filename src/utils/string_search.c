#include <stdlib.h>
#include <string.h>
#include "string_utils.h"

unsigned int string_search(const s_string *s, const char *needle)
{
    return strstr(s->buf, needle) - s->buf;
}

s_string *string_extract(const s_string *str, size_t start, size_t end)
{
    s_string *res = string_create(0);
    if (end == 0 || end > str->len)
        end = str->len;
    for (size_t i = start; i < end; ++i)
        string_putc(res, str->buf[i]);
    return res;
}
