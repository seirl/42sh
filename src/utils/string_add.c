#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "string_utils.h"

void string_putc(s_string *s, char c)
{
    if (s->len >= s->max_len - 1)
        string_resize(s);
    s->buf[(s->len)++] = c;
    s->buf[s->len] = 0;
}

void string_cat(s_string *s1, s_string *s2)
{
    for (unsigned i = 0; i < s2->len; ++i)
        string_putc(s1, s2->buf[i]);
}

void string_puts(s_string *s, const char *str)
{
    if (str == NULL)
        return;
    for (int i = 0; str[i]; ++i)
        string_putc(s, str[i]);
}

int string_insertc(s_string *s, char c, size_t index)
{
    if (s == NULL)
        return -1;
    if (index > s->len)
        return -2;

    if (s->len >= s->max_len - 1)
        string_resize(s);
    for (size_t i = s->len; i > index; i--)
        s->buf[i] = s->buf[i - 1];

    s->buf[index] = c;
    s->buf[++(s->len)] = 0;
    return 0;
}

s_string *string_vcat(const char *s1, ...)
{
    s_string *r = string_create(0);
    va_list ap;
    va_start(ap, s1);
    for (const char *c = s1; c; c = va_arg(ap, const char *))
        string_puts(r, c);
    return r;
}
