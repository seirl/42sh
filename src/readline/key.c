#include <unistd.h>
#include <stdio.h>
#include "key.h"
#include "bracket_key.h"

char getkey(s_term *term)
{
    char c;
    read(STDIN_FILENO, &c, sizeof (char));
    if (c == 27) // \x1b: escape
    {
        read(STDIN_FILENO, &c, sizeof (char));
        if (c == '[')
            handle_bracket_char(term);
        return 0;
    }
    return c;
}
