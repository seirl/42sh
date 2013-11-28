#include "special_line_manipulation_keys.h"
#include "key.h"
#include "bracket_key.h"
#include "wrapper.h"
#include "konami.h"

e_next_action do_ctrl_a(s_shell *shell, s_term *term)
{
    while (term->input_index > 0)
        handle_bracket_key(shell, BRACKET_LEFT, term);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

e_next_action do_ctrl_e(s_shell *shell, s_term *term)
{
    while (term->input_index < term->input->len)
        handle_bracket_key(shell, BRACKET_RIGHT, term);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

e_next_action do_ctrl_k(s_term *term)
{
    my_tputs(tgetstr("ce", NULL));
    string_del_from_end(term->input, term->input->len - term->input_index);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

e_next_action do_ctrl_l(s_term *term)
{
    my_tputs(tgetstr("cl", NULL));
    printf("%s%s", term->prompt, term->input->buf);
    fflush(stdout);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}

e_next_action do_ctrl_c(s_term *term)
{
    printf("%s\n", "^C");
    printf("%s", term->prompt);
    string_reset(term->input);
    term->input_index = 0;
    fflush(stdout);
    konami_next(term, KONAMI_NOTHING);
    return CONTINUE;
}
