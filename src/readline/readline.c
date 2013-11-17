#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "terminal.h"
#include "key.h"
#include "key_signal.h"

#include "wrapper.h"
s_string *readline()
{
    s_term *term = term_get();
    char c;
    //my_tputs(tgetstr("sc", &term->bp));
    //my_tputs(tgetstr("ve", &term->bp));
    while (1)
    {
        c = getkey(term);
        if (c == 0)
            continue;
        if (signal_key(c))
            break;
        if (isprint(c))
            printf("%c", c);
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
