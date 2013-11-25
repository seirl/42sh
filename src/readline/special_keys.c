#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"
#include "wrapper.h"

static e_next_action do_backspace(s_term *term)
{
    if (term->input_index > 0)
    {
        term->input_index--;
        string_del_nth(term->input, term->input_index, 1);

        // cursor left
        my_tputs(tgetstr("le", NULL));
        // delete mode
        my_tputs(tgetstr("dm", NULL));
        // delete char
        my_tputs(tgetstr("dc", NULL));
        // edition mode
        my_tputs(tgetstr("ed", NULL));
    }
    return CONTINUE;
}

static e_next_action do_ctrl_d(s_term *term)
{
    if (term->input->len != 0)
        return CONTINUE;
    return EOI;
}

static e_next_action do_enter(s_term *term)
{
    // TODO: return only when not escaped with '\'
    term += 1;
    return RETURN;
}


e_next_action handle_special_key(s_shell *shell, s_term *term, e_special_key key)
{
#define X(Name, Code1, Code2, Handler)              \
    if (key == Name)                                \
        return Handler;
#include "special_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}

e_next_action handle_special_char(s_shell *shell, s_term *term, char c)
{
#define X(Name, Code1, Code2, Handler)             \
    if (c == Code1 || (Code2 != 0 && c == Code2))  \
        return Handler;
#include "special_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}
