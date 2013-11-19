#include <unistd.h>
#include "key.h"
#include "bracket_key.h"
#include "wrapper.h"
#include "history.h"

static void do_right(s_term *term)
{
    if (term->input_index < term->input->len)
    {
        my_tputs(tgetstr("nd", NULL));
        term->input_index++;
    }
}

static void do_left(s_term *term)
{
    if (term->input_index > 0)
    {
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
}

static void do_up(s_term *term)
{
    // TODO: fix "undefined reference to get_history"
    (void)term;
    // term->hist_current = get_history(++(term->hist_pos));
}

static e_next_action handle_bracket_key(e_bracket_key key, s_term *term)
{
    switch (key)
    {
#define X(Name, Char1, Char2, Fun)  \
        case Name:                  \
            Fun;                    \
            break;
#include "bracket_key.def"
#undef X
        default:
            break;
    }
    return CONTINUE;
}

e_next_action handle_bracket_char(s_term *term)
{
    char c;
    char c2 = -1;
    if (read(STDIN_FILENO, &c, sizeof (char)) == -1)
        return ERROR;
#define X(Name, Char1, Char2, Fun)                                  \
    if (c == Char1)                                                 \
    {                                                               \
        if (Char2 == 0)                                             \
            return handle_bracket_key(Name, term);                  \
        else                                                        \
        {                                                           \
            if (c2 == -1)                                           \
                if (read(STDIN_FILENO, &c2, sizeof (char)) == -1)   \
                    return ERROR;                                   \
            if (c2 == Char2)                                        \
                return handle_bracket_key(Name, term);              \
        }                                                           \
    }
#include "bracket_key.def"
#undef X
    return CONTINUE;
}
