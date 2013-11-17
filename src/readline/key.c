#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "key.h"
#include "bracket_key.h"
#include "special_keys.h"

e_next_action getkey(s_term *term, char *c)
{
    read(STDIN_FILENO, c, sizeof (char));
    if (isprint(*c))
        return PRINT;
    if (*c == 27) // \x1b: escape
    {
        read(STDIN_FILENO, c, sizeof (char));
        if (*c == '[')
            handle_bracket_char(term);
        return CONTINUE;
    }
    return handle_special_char(term, *c);
}
