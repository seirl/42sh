#include <stdlib.h>
#include "string_utils.h"

void string_putc(s_string *s, char c)
{
    if (s->len >= s->max_len - 1)
        string_resize(s);
    s->buf[(s->len)++] = c;
    s->buf[s->len] = 0;
}

void string_ungetc(s_string *s)
{
    if (s->len == 0)
        return;
    s->buf[(--s->len)] = 0;
}

void string_cat(s_string *s1, s_string *s2)
{
    char c;
    for (unsigned i = 0; i < s2->len; ++i)
    {
        c = s2->buf[i];
        string_putc(s1, c);
    }
}

void string_puts(s_string *s, char *str)
{
    if (str == NULL)
        return;
    for (int i = 0; str[i]; ++i)
        string_putc(s, str[i]);
}
