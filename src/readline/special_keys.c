#include <stdio.h>
#include "env.h"
#include "key.h"
#include "special_keys.h"
#include "wrapper.h"
#include "terminal.h"
#include "konami.h"
#include "autocompletion.h"

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
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

static e_next_action do_ctrl_d(s_term *term)
{
    if (term->input->len != 0)
    {
        konami_next(term, KONAMI_NOTHING);
        return CONTINUE;
    }
    konami_next(term, KONAMI_NOTHING);
    return EOI;
}

static e_next_action do_enter(s_term *term)
{
    // TODO: return only when not escaped with '\'
    term += 1;
    return RETURN;
}

static e_next_action do_ctrl_w(s_term *term)
{
    size_t *i = &(term->input_index);
    while (*i > 0 && input_is_blank(term, *i - 1))
        do_backspace(term);
    while (*i > 0 && !input_is_blank(term, *i - 1))
        do_backspace(term);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

static e_next_action do_ctrl_u(s_term *term)
{
    my_tputs(tgetstr("dm", NULL));
    while (term->input_index)
    {
        my_tputs(tgetstr("le", NULL));
        my_tputs(tgetstr("dc", NULL));
        term->input_index--;
        string_del_nth(term->input, term->input_index, 1);
    }
    my_tputs(tgetstr("ed", NULL));
    konami_next(term, KONAMI_NOTHING);

    return CONTINUE;
}

static e_next_action do_tab(s_shell *shell, s_term *term)
{
    autocomplete(shell, term);
    return CONTINUE;
}

e_next_action handle_special_key(s_shell *shell, s_term *term,
                                 e_special_key key)
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
