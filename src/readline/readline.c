#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "readline.h"

#include "env.h"
#include "key.h"
#include "string_utils.h"
#include "terminal.h"
#include "wrapper.h"
#include "prompt.h"
#include "bracket_key.h"
#include "konami.h"

static int is_blank(s_string *line)
{
    if (!line)
        return 1;
    for (size_t i = 0; i < line->len; i++)
        if (line->buf[i] != ' ' && line->buf[i] != '\t')
            return 0;
    return 1;
}

static s_string *readline_close(s_shell *shell, s_term *term,
                                e_next_action ret)
{
    s_string *input = (ret == EOI) ? NULL : string_duplicate(term->input);
    // TODO: what to do when ret == ERROR ?
    term_close();
    if (input)
        printf("\n");
    fflush(stdout);
    if (!is_blank(input))
        history_add(shell, string_duplicate(input));
    return input;
}

void readline_update_line(s_term *term)
{
    while (term->input_index > 0)
    {
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
    my_tputs(tgetstr("ce", NULL));
    printf("%s", term->input->buf);
    fflush(stdout);
    term->input_index = term->input->len;
}

void readline_do_print(s_shell *shell, char c)
{
    s_term *term = term_get(shell);
    // Insert the character in the current line buffer
    string_insertc(term->input, c, term->input_index);
    term->input_index++;

    // Inform the konami module
    if (c == 'a')
    {
        // The konami sequence was completed, abort printing.
        if (konami_next(term, KONAMI_A))
            return;
    }
    else if (c == 'b')
        konami_next(term, KONAMI_B);
    else
        konami_next(term, KONAMI_NOTHING);

    // Pass the terminal into insert mode, insert the character then
    // return to edit mode.
    my_tputs(tgetstr("im", NULL));
    printf("%c", c);
    fflush(stdout);
    my_tputs(tgetstr("ei", NULL));
}

s_string *readline(s_shell *shell, char *prompt)
{
    s_term *term = term_get(shell);
    konami_next(term, KONAMI_NOTHING);
    if (term == NULL)
        return NULL;
    char c;

    s_string *s_prompt = string_create_from(env_get(shell, prompt));
    prompt_expand(shell, &s_prompt);
    printf("%s%s", s_prompt->buf, term->input->buf);
    term->prompt = s_prompt->buf;
    fflush(stdout);
    for (size_t i = term->input->len; i > term->input_index; i--)
        my_tputs(tgetstr("le", NULL));
    e_next_action ret;
    while (1)
    {
        ret = getkey(shell, term, &c);
        if (ret == CONTINUE)
            continue;
        if (ret == RETURN || ret == ERROR || ret == EOI)
            break;
        if (ret == PRINT && isprint(c))
            readline_do_print(shell, c);
    }

    string_free(s_prompt);
    return readline_close(shell, term, ret);
}
