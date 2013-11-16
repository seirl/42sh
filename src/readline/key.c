#include <unistd.h>
#include <stdio.h>
#include "key.h"

int handle_arrow(char c)
{
    e_key k;
    if (c != '[')
        return 0;
    char next = getkey();
#define X(Char, Res)              \
    if (next == Char)             \
    {                             \
        k = Res;                  \
        printf("%s", #Res);       \
        return 1;                 \
    }
#include "arrow.def" 
#undef X
    (void)k;
    return 1; //ERROR ?
}

char getkey()
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
            if (handle_arrow(c))
                return 0;
        }
        else
            break;
    }
    return c;
}
