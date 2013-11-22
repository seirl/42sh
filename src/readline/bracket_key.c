#include <unistd.h>
#include "key.h"
#include "bracket_key.h"
#include "wrapper.h"
#include "history.h"

static void update_line(s_term *term)
{
    handle_special_key(term, CTRL_A);
    my_tputs(tgetstr("ce", NULL));
    printf("%s", term->input->buf);
    fflush(stdout);
}

static e_next_action do_right(s_term *term)
{
    if (term->input_index < term->input->len)
    {
        // Move cursor to the left
        my_tputs(tgetstr("nd", NULL));
        term->input_index++;
    }
    return CONTINUE;
}

static e_next_action do_left(s_term *term)
{
    if (term->input_index > 0)
    {
        // Move cursor to the right
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
    return CONTINUE;
}

static e_next_action do_up(s_term *term)
{
    if (term->hist_pos + 1 >= history_size())
        return CONTINUE;

    term->hist_pos++;
    term->input = history_get(term->hist_pos)->line;
    update_line(term);
    term->input_index = term->input->len;
    return CONTINUE;
}

static e_next_action do_down(s_term *term)
{
    if (term->hist_pos < 0)
        return CONTINUE;

    term->hist_pos--;
    if (term->hist_pos == -1)
        term->input = term->backup_input;
    else
        term->input = history_get(term->hist_pos)->line;
    update_line(term);
    term->input_index = term->input->len;
    return CONTINUE;
}

e_next_action handle_bracket_key(e_bracket_key key, s_term *term)
{
#define X(Name, Char1, Char2, Handler)  \
        if (key == Name)                \
            return Handler;
#include "bracket_key.def"
#undef X
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
