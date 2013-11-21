#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "key.h"
#include "bracket_key.h"
#include "special_keys.h"

e_next_action getkey(s_shell *shell, s_term *term, char *c)
{
    if (read(STDIN_FILENO, c, sizeof (char)) == -1)
        return ERROR;

    if (isprint(*c))
        return PRINT;

    // \x1b: escape
    if (*c == 27)
    {
        if (read(STDIN_FILENO, c, sizeof (char)) == -1)
            return ERROR;

        if (*c == '[')
            handle_bracket_char(shell, term);
        return CONTINUE;
    }
    return handle_special_char(term, *c);
}
