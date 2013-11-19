#include <unistd.h>
#include "key.h"
#include "bracket_key.h"
#include "wrapper.h"
#include "history.h"

static void do_right(s_term *term)
{
    if (term->input_index < term->input->len)
    {
        // Move cursor to the left
        my_tputs(tgetstr("nd", NULL));
        term->input_index++;
    }
}

static void do_left(s_term *term)
{
    if (term->input_index > 0)
    {
        // Move cursor to the right
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
}

static void update_line(s_term *term, char *line)
{
    while (term->input_index < term->input->len)
        my_tputs(tgetstr("nd", NULL));

    my_tputs(tgetstr("dm", NULL));
    while (term->input_index > 0)
    {
        my_tputs(tgetstr("le", NULL));
        my_tputs(tgetstr("dc", NULL));
    }
    my_tputs(tgetstr("ed", NULL));

    printf("%s", line);
}

static void do_up(s_term *term)
{
    if (!USE_HIST || term->hist_pos >= history_size())
        return;

    term->hist_pos++;
    term->hist_current = history_get(term->hist_pos);
    update_line(term, term->hist_current->line->buf);
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
