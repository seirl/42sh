#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "terminal.h"
#include "key.h"
#include "env.h"

#include "wrapper.h"

static s_string *readline_close(s_term *term, e_next_action ret)
{
    s_string *input = (ret == ERROR) ? NULL : string_duplicate(term->input);
    term_close();
    fflush(stdout);
    return input;
}

s_string *readline(char *prompt)
{
    s_term *term = term_get();
    if (term == NULL)
        return NULL;
    char c;

    printf("%s", prompt);
    fflush(stdout);
    e_next_action ret;
    while (1)
    {
        ret = getkey(term, &c);
        if (ret == CONTINUE)
            continue;
        if (ret == RETURN || ret == ERROR)
            break;
        if (ret == PRINT && isprint(c))
        {
            string_insertc(term->input, c, term->input_index);
            term->input_index++;
            my_tputs(tgetstr("im", NULL));
            printf("%c", c);
            fflush(stdout);
            my_tputs(tgetstr("ei", NULL));
        }
    }

    return readline_close(term, ret);
}
