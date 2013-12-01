#include <stdio.h>
#include "env.h"
#include "key.h"
#include "bracket_key.h"
#include "escape_keys.h"
#include "terminal.h"
#include "readline.h"
#include "wrapper.h"
#include "konami.h"
#include "alt_q.h"

static e_next_action do_b(s_shell *shell, s_term *term)
{
    size_t *i = &(term->input_index);
    while (*i > 0 && input_is_blank(term, *i - 1))
        handle_bracket_key(shell, BRACKET_LEFT, term);
    while (*i > 0 && !input_is_blank(term, *i - 1))
        handle_bracket_key(shell, BRACKET_LEFT, term);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

static e_next_action do_f(s_shell *shell, s_term *term)
{
    size_t len = term->input->len;
    size_t *i = &(term->input_index);
    while (*i < len && input_is_blank(term, *i))
        handle_bracket_key(shell, BRACKET_RIGHT, term);
    while (*i < len && !input_is_blank(term, *i))
        handle_bracket_key(shell, BRACKET_RIGHT, term);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

static e_next_action do_dot(s_shell *shell, s_term *term)
{
    s_string *last_line = string_duplicate(history_get(shell, 0)->line);
    size_t end = last_line->len;
    size_t i = end;
    while (i && (last_line->buf[i] == ' ' || last_line->buf[i] == '\t'))
        i--;
    while (i && last_line->buf[i] != ' ' && last_line->buf[i] != '\t')
        i--;

    if (last_line->buf[i] == ' ' || last_line->buf[i] == '\t')
        i++;

    my_tputs(tgetstr("im", NULL));
    while (i < end)
    {
        string_insertc(term->input, last_line->buf[i], term->input_index);
        term->input_index++;

        printf("%c", last_line->buf[i]);
        fflush(stdout);
        i++;
    }
    my_tputs(tgetstr("ei", NULL));

    string_free(last_line);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

static e_next_action do_h(s_term *term)
{
    size_t input_index = term->input_index;
    s_string *old_input = term->input;
    do_alt_q(term, input_index);

    string_puts(term->input, "man ");
    for (int i = 0; old_input->buf[i] != ' ' && old_input->buf[i] != '\t'; i++)
        string_putc(term->input, old_input->buf[i]);
    return RETURN;
}

static e_next_action do_e(s_shell *shell, s_term *term)
{
    size_t input_index = term->input_index;
    s_string *old_input = term->input;
    do_alt_q(term, input_index);

    string_puts(term->input, env_get(shell, "EDITOR"));
    string_putc(term->input, ' ');
    string_puts(term->input, old_input->buf);
    return RETURN;
}

e_next_action handle_escape_key(s_shell *shell, s_term *term, e_escape_key key)
{
#define X(Name, Code1, Code2, Handler)              \
    if (key == Name)                                \
        return Handler;
#include "escape_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}

e_next_action handle_escape_char(s_shell *shell, s_term *term, char c)
{
#define X(Name, Code1, Code2, Handler)             \
    if (c == Code1 || (Code2 != 0 && c == Code2))  \
        return Handler;
#include "escape_keys.def"
#undef X
    // Unrecognized key, ignore it.
    return PRINT;
}
