#include "string_utils.h"

void string_reset(s_string *s)
{
    s->read_pos = 0;
    s->len = 0;
}

char *string_nullterminated(s_string *s)
{
    if (s->buf[s->len] != '\0')
    {
        if (s->len == s->max_len)
            string_resize(s);
        s->buf[s->len] = '\0';
    }

    return s->buf;
}
