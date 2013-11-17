#include <unistd.h>
#include <stdio.h>
#include "key.h"
#include "bracket_key.h"
#include "special_keys.h"

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
    else
    {
        int ret = handle_special_char(term, c);
        switch (ret)
        {
            case 0:
                return c;
            case 1:
                return 0;
            case 2:
                return -1;
        }
    }
    return c;
}
