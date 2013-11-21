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

static void update_line(s_term *term, size_t end)
{
    while (term->input_index < end)
        my_tputs(tgetstr("nd", NULL));

    my_tputs(tgetstr("dm", NULL));
    while (term->input_index > 0)
    {
        my_tputs(tgetstr("le", NULL));
        my_tputs(tgetstr("dc", NULL));
        term->input_index--;
    }
    my_tputs(tgetstr("ed", NULL));

    printf("%s", term->input->buf);
    fflush(stdout);
}

static void do_up(s_shell *shell, s_term *term)
{
    if (!USE_HIST || term->hist_pos + 1 >= history_size(shell))
        return;

    size_t end = term->input->len;
    term->hist_pos++;
    term->input = history_get(shell, term->hist_pos)->line;
    update_line(term, end);
    term->input_index = term->input->len;
}

static void do_down(s_shell *shell, s_term *term)
{
    if (!USE_HIST || term->hist_pos < 0)
        return;

    size_t end = term->input->len;
    term->hist_pos--;
    if (term->hist_pos == -1)
        term->input = term->backup_input;
    else
        term->input = history_get(shell, term->hist_pos)->line;
    update_line(term, end);
    term->input_index = term->input->len;
}

static e_next_action handle_bracket_key(s_shell *shell, e_bracket_key key,
                                        s_term *term)
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

e_next_action handle_bracket_char(s_shell *shell, s_term *term)
{
    char c;
    char c2 = -1;
    if (read(STDIN_FILENO, &c, sizeof (char)) == -1)
        return ERROR;
#define X(Name, Char1, Char2, Fun)                                \
    if (c == Char1)                                               \
    {                                                             \
        if (Char2 == 0)                                           \
            return handle_bracket_key(shell, Name, term);         \
        else                                                      \
        {                                                         \
            if (c2 == -1)                                         \
                if (read(STDIN_FILENO, &c2, sizeof (char)) == -1) \
                    return ERROR;                                 \
            if (c2 == Char2)                                      \
                return handle_bracket_key(shell, Name, term);     \
        }                                                         \
    }
#include "bracket_key.def"
#undef X
    return CONTINUE;
}
