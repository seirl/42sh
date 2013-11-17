#include <stdio.h>
#include "env.h"
#include "special_keys.h"

static int handle_special_key(e_special_key key, s_term *term)
{
    (void)term;
    switch (key)
    {
        case ENTER:
            printf("\n");
            printf("%s", env_get("PS1"));
            fflush(stdout);
            break;
        case CTRL_C:
        case CTRL_D:
            return 2;
            break;
        default:
            break;
    }
    return 1;
}

int handle_special_char(s_term *term, char c)
{
#define X(Name, Code)                             \
    if (c == Code)                                \
        return handle_special_key(Name, term);
#include "special_keys.def"
#undef X
    return 0;
}
