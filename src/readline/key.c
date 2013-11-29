#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include "key.h"
#include "bracket_key.h"
#include "special_keys.h"
#include "escape_keys.h"

e_next_action handle_key(s_shell *shell, s_term *term, char *c)
{
    if (isprint(*c))
        return PRINT;

    // \x1b: escape
    if (*c == 27)
    {
        if (read(STDIN_FILENO, c, sizeof (char)) == -1)
            return ERROR;

        if (*c == '[')
            handle_bracket_char(shell, term);
        else
            handle_escape_char(shell, term, *c);
        return CONTINUE;
    }
    return handle_special_char(shell, term, *c);
}

e_next_action getkey(s_shell *shell, s_term *term, char *c)
{
    if (read(STDIN_FILENO, c, sizeof (char)) == -1)
        return ERROR;

    return handle_key(shell, term, c);
}
