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
    history_close();
    s_string *input = (ret == ERROR) ? NULL : string_duplicate(term->input);
    term_close();
    printf("\n");
    fflush(stdout);
    return input;
}

static void do_print(char c)
{
    s_term *term = term_get();
    // Insert the character in the current line buffer
    string_insertc(term->input, c, term->input_index);
    term->input_index++;

    // Pass the terminal into insert mode, insert the character then
    // return to edit mode.
    my_tputs(tgetstr("im", NULL));
    printf("%c", c);
    fflush(stdout);
    my_tputs(tgetstr("ei", NULL));
}

s_string *readline(char *prompt)
{
    s_term *term = term_get();
    term->prompt = prompt;
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
            do_print(c);
    }

    return readline_close(term, ret);
}
