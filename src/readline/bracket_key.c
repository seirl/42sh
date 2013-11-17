#include <unistd.h>
#include "bracket_key.h"
#include "wrapper.h"

#include <stdio.h>
static int handle_bracket_key(e_bracket_key key, s_term *term)
{
    (void)key;
    switch (key)
    {
        case CHAR_UP:
            break;
        case CHAR_DOWN:
            break;
        case CHAR_RIGHT:
            my_tputs(tgetstr("nd", &term->bp));
            break;
        case CHAR_LEFT:
            my_tputs(tgetstr("le", &term->bp));
            break;
        default:
            break;
    }
    return 1;
}

int handle_bracket_char(s_term *term)
{
    char c;
    char c2 = -1;
    read(STDIN_FILENO, &c, sizeof (char));
#define X(Name, Char1, Char2)                               \
    if (c == Char1)                                         \
    {                                                       \
        if (Char2 == 0)                                     \
            return handle_bracket_key(Name, term);          \
        else                                                \
        {                                                   \
            if (c2 == -1)                                   \
                read(STDIN_FILENO, &c2, sizeof (char));     \
            if (c2 == Char2)                                \
                return handle_bracket_key(Name, term);      \
        }                                                   \
    }
#include "bracket_key.def"
#undef X
    return 0;
}
