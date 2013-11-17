#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"
#include "wrapper.h"

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
            fflush(stdout);
            return CONTINUE;
        case CTRL_D:
            if (term->input->len != 0)
                return CONTINUE;
            string_puts(term->input, "exit");
            return RETURN;
        case BACKSPACE:
            if (term->input->len > 0)
            {
                string_del_from_end(term->input, 1);
                my_tputs(tgetstr("le", &term->bp)); //cursor left
                my_tputs(tgetstr("dm", &term->bp)); //delete mode
                my_tputs(tgetstr("dc", &term->bp)); //delete char
                my_tputs(tgetstr("ed", &term->bp)); //edition mode
            }
        default:
            return CONTINUE; // Unrecognized key, just ignore it.
    }
}

e_next_action handle_special_char(s_term *term, char c)
{
#define X(Name, Code)                             \
    if (c == Code)                                \
        return handle_special_key(Name, term);
#include "special_keys.def"
#undef X
    return PRINT; // Unrecognized key, just ignore it.
}
