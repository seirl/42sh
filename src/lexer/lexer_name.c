#include <ctype.h>
#include "lexer_private.h"

int is_valid_name(const s_string *str)
{
    const char *name = str->buf;
    if (isdigit(name[0]))
        return 0;
    while (*name)
    {
        if (!isalpha(*name))
            return 0;
        ++name;
    }
    return 1;
}
