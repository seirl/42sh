#include <stdio.h>
#include "env.h"
#include "key.h"
#include "bracket_key.h"
#include "escape_keys.h"
#include "terminal.h"

static e_next_action do_b(s_shell *shell, s_term *term)
{
    size_t *i = &(term->input_index);
    while (*i > 0 && input_is_blank(term, *i))
        handle_bracket_key(shell, BRACKET_LEFT, term);
    while (*i > 0 && !input_is_blank(term, *i))
        handle_bracket_key(shell, BRACKET_LEFT, term);
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
    return CONTINUE;
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
