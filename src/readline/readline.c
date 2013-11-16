#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "terminal.h"
#include "key.h"
#include "key_signal.h"

s_string *readline()
{
    s_term *term = term_get();
    (void)term;
    char c;
    while (1)
    {
        c = getkey();
        if (signal_key(c))
            break;
        if (isprint(c))
            printf("%c", c);
        else
            printf("%d", c);
        fflush(stdout);
        //else if (readline_printable(c))
        //{
        //    string_putc(buffer, c);
        //    index++;
        //}
        //else if (c == '\x1b') /** ESC (escape) == ^[ */
        //    handle_special(buffer, c);
    }

    term_close();
    return NULL;
}
