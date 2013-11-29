#include "expand.h"

void remove_special_backslash(s_string *str)
{
    for (int i = 0; str->buf[i]; ++i)
    {
        if (str->buf[i] == '\\')
        {
            char c = str->buf[i + 1];
            if (c == '$' || c == '`' || c == '\"' || c == '\n')
            {
                for (int j = 0; str->buf[i + j]; ++j)
                    str->buf[i + j] = str->buf[i + j + 1];
                str->len -= 1;
                if (c == '\n')
                {
                    for (int j = 0; str->buf[i + j]; ++j)
                        str->buf[i + j] = str->buf[i + j + 1];
                    str->len -= 1;
                }
            }
        }
    }
}
