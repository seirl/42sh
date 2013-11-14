#include <stdlib.h>
#include <string.h>
#include "string_utils.h"

s_string *string_replace(const s_string *s, const char *pat, const char *rep)
{
    s_string *sr = string_create(0);
    size_t len_pat = strlen(pat);
    for (unsigned int i = 0; i < s->len; i++)
    {
        if (!strncmp(s->buf + i, pat, len_pat))
        {
            string_puts(sr, rep);
            i += len_pat;
        }
        else
            string_putc(sr, s->buf[i]);
    }
    return sr;
}

void string_eat_spaces(s_string *s)
{
    char c;
    while ((c = string_getc(s)) == ' ' && c != 0)
        continue;
    if (c != 0)
        string_ungetc(s);
}
