#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "terminal.h"
#include "key.h"
#include "env.h"

#include "wrapper.h"
s_string *readline()
{
    s_term *term = term_get();
    char c;

    env_set("42sh$ ", "PS1");
    printf("%s", env_get("PS1"));
    fflush(stdout);
    while (1)
    {
        c = getkey(term);
        if (c == 0)
            continue;
        if (c == -1)
            return NULL;
        if (isprint(c))
        {
            printf("%c", c);
            string_putc(term->input, c);
        }
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
    env_free();
    return NULL;
}
