#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"

static e_next_action handle_special_key(e_special_key key, s_term *term)
{
    (void)term;
    switch (key)
    {
        case ENTER:
            return RETURN;
            printf("%s", env_get("PS1"));
            fflush(stdout);
            return CONTINUE;
        case CTRL_D:
            return 2;
            break;
        default:
            return CONTINUE; // Unrecognized key, just ignore it.
    }
}

e_next_action handle_special_char(s_term *term, char c)
{
#define X(Name, Code)                             \
    if (c == Code)                                \
        return handle_special_key(Name, term);
#include "special_keys.def"
#undef X
    return CONTINUE; // Unrecognized key, just ignore it.
}
