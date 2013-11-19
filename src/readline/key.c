#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "key.h"
#include "bracket_key.h"
#include "special_keys.h"

e_next_action getkey(s_term *term, char *c)
{
    if (read(STDIN_FILENO, c, sizeof (char)) == -1)
        return ERROR;

    if (isprint(*c))
        return PRINT;

    if (*c == 27) // \x1b: escape
    {
        if (read(STDIN_FILENO, c, sizeof (char)) == -1)
            return ERROR;

        if (*c == '[')
            handle_bracket_char(term);
        return CONTINUE;
    }
    return handle_special_char(term, *c);
}
