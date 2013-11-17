#include <unistd.h>
#include <stdio.h>
#include "key.h"
#include "cursor_handler.h"

int handle_arrow(char c, s_term *term)
{
    e_key k;
    if (c != '[')
        return 0;
    char next = getkey(term);
#define X(Char, Res)               \
    if (next == Char)              \
    {                              \
        k = Res;                   \
        if (Res == KEY_CURS_LEFT)  \
            curs_left(term);       \
        if (Res == KEY_CURS_RIGHT) \
            curs_right(term);      \
        return 1;                  \
    }
#include "arrow.def" 
#undef X
    (void)k;
    return 1; //ERROR ?
}

char getkey(s_term *term)
{
    char c;
    int escape = 0;
    while (1)
    {
        read(STDIN_FILENO, &c, sizeof (char));
        if (c == '\x1b')
            escape = 1;
        else if (escape)
        {
            if (handle_arrow(c, term))
                return 0;
        }
        else
            break;
    }
    return c;
}
