#include <stdio.h>
#include "special_keys.h"

static void handle_special_key(e_special_key key, s_term *term)
{
    (void)term;
    switch (key)
    {
        case ENTER:
            printf("\n");
            break;
        default:
            break;
    }
}

int handle_special_char(s_term *term, char c)
{
#define X(Name, Code)                      \
    if (c == Code)                         \
    {                                      \
        handle_special_key(Name, term);    \
        return 1;                          \
    }
#include "special_keys.def"
#undef X
    return 0;
}
