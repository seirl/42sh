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
    return CONTINUE;
}

static e_next_action do_ctrl_c(s_term *term)
{
    printf("%s\n", "^C");
    printf("%s", term->prompt);
    string_reset(term->input);
    term->input_index = 0;
    fflush(stdout);
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

static e_next_action do_ctrl_a(s_term *term)
{
    while (term->input_index > 0)
        handle_bracket_key(BRACKET_LEFT, term);
    return CONTINUE;
}

static e_next_action do_ctrl_e(s_term *term)
{
    while (term->input_index < term->input->len)
        handle_bracket_key(BRACKET_RIGHT, term);
    return CONTINUE;
}

static e_next_action do_ctrl_k(s_term *term)
{
    my_tputs(tgetstr("ce", NULL));
    string_del_from_end(term->input, term->input->len - term->input_index);
    return CONTINUE;
}

e_next_action handle_special_char(s_term *term, char c)
{
#define X(Name, Code1, Code2, Handler)             \
    if (c == Code1 || (Code2 != 0 && c == Code2))  \
        return Handler;
#include "special_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}
