#include <string.h>
#include "string_utils.h"

int string_equal(const s_string *s, const char *pattern)
{
    size_t i = 0;
    for (; i < s->len; ++i)
        if (pattern[i] == '\0' || s->buf[i] != pattern[i])
            return 0;

    return pattern[i] == '\0';
}

int string_begins_with(const s_string *s, const char *pattern)
{
    for (int i = 0; pattern[i]; ++i)
    {
        if (pattern[i] != s->buf[i])
            return 0;
    }
    return 1;
}

int string_ends_with(const s_string *s, const s_string *pattern)
{
    s_string *end = string_extract(s, s->len - pattern->len, 0);
    int ret = strcmp(end->buf, pattern->buf);
    string_free(end);
    return ret == 0;
}

static int signed_int(size_t i)
{
    return i;
}

char string_index(const s_string *s, int index)
{
    if (index < 0 || index > signed_int(s->len))
        return '\0';
    return s->buf[index];
}
