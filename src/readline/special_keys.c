#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"
#include "wrapper.h"

static void do_backspace(s_term *term)
{
    if (term->input->len > 0)
    {
        string_del_from_end(term->input, 1);
        my_tputs(tgetstr("le", NULL)); //cursor left
        my_tputs(tgetstr("dm", NULL)); //delete mode
        my_tputs(tgetstr("dc", NULL)); //delete char
        my_tputs(tgetstr("ed", NULL)); //edition mode
    }
}

static e_next_action handle_special_key(e_special_key key, s_term *term)
{
    switch (key)
    {
        case ENTER:
            return RETURN;
        case CTRL_C:
            printf("%s\n", "^C");
            printf("%s", env_get("PS1"));
            string_reset(term->input);
            term->input_index = 0;
            fflush(stdout);
            return CONTINUE;
        case CTRL_D:
            if (term->input->len != 0)
                return CONTINUE;
            string_puts(term->input, "exit");
            printf("exit\n");
            return RETURN;
        case BACKSPACE:
            do_backspace(term);
        default:
            return CONTINUE; // Unrecognized key, just ignore it.
    }
}

e_next_action handle_special_char(s_term *term, char c)
{
#define X(Name, Code1, Code2)                      \
    if (c == Code1 || (Code2 != 0 && c == Code2))  \
        return handle_special_key(Name, term);
#include "special_keys.def"
#undef X
    return PRINT; // Unrecognized key, just ignore it.
}
