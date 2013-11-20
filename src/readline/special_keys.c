#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"
#include "wrapper.h"

static void do_backspace(s_term *term)
{
    if (term->input_index > 0)
    {
        term->input_index--;
        string_del_nth(term->input, term->input_index);

        // cursor left
        my_tputs(tgetstr("le", NULL));
        // delete mode
        my_tputs(tgetstr("dm", NULL));
        // delete char
        my_tputs(tgetstr("dc", NULL));
        // edition mode
        my_tputs(tgetstr("ed", NULL));
    }
}

static e_next_action handle_special_key(e_special_key key, s_term *term)
{
    switch (key)
    {
        case ENTER:
            return RETURN; // TODO: return only if no '\' escape.
        case CTRL_C:
            printf("%s\n", "^C");
            printf("%s", term->prompt);
            string_reset(term->input);
            term->input_index = 0;
            fflush(stdout);
            return CONTINUE;
        case CTRL_D:
            if (term->input->len != 0)
                return CONTINUE;
            string_puts(term->input, "exit");
            printf("exit");
            return RETURN;
        case BACKSPACE:
            do_backspace(term);
        default:
            // Unrecognized key, ignore it.
            return CONTINUE;
    }
}

e_next_action handle_special_char(s_term *term, char c)
{
#define X(Name, Code1, Code2)                      \
    if (c == Code1 || (Code2 != 0 && c == Code2))  \
        return handle_special_key(Name, term);
#include "special_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}
