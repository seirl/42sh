#include "string_utils.h"

int string_equal(const s_string *s, const char *pattern)
{
    size_t i = 0;
    for (; i < s->len; ++i)
        if (pattern[i] == '\0' || s->buf[i] != pattern[i])
            return 0;

    return pattern[i] == '\0';
}
