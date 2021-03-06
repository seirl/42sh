#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "smalloc.h"
#include "autocompletion.h"
#include "terminal.h"
#include "env.h"
#include "match.h"
#include "readline.h"
#include "wrapper.h"
#include "escape_keys.h"
#include "autocomp_tools.h"
#include "autocomp_bins.h"
#include "autocomp_paths.h"

static void replace_word(s_term *term, s_string *word)
{
    if (!word)
        return;

    size_t start = start_word(term);
    size_t end = end_word(term);
    s_string *new_input = string_create(term->input->len);

    for (size_t i = 0; i < start; i++)
        string_putc(new_input, term->input->buf[i]);
    for (size_t i = 0; i < word->len; i++)
        string_putc(new_input, word->buf[i]);
    for (size_t i = end; i < term->input->len; i++)
        string_putc(new_input, term->input->buf[i]);

    sfree(term->input->buf);
    term->input->buf = new_input->buf;
    term->input->len = new_input->len;
    term->input->max_len = new_input->max_len;
    term->input->read_pos = new_input->read_pos;
    sfree(new_input);

    readline_update_line(term);
    while (term->input_index > start + word->len)
    {
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
}

static int is_path(s_term *term)
{
    size_t start = start_word(term);
    return (term->input->buf[start] == '.' || term->input->buf[start] == '/');
}

int autocomplete(s_shell *shell, s_term *term)
{
    size_t i = term->input_index;
    if (!is_blank(term->input->buf[i]))
        handle_escape_key(shell, term, ESCAPE_F);
    i = term->input_index;
    if (i && is_blank(term->input->buf[i - 1]))
        return 0;

    s_string *s = NULL;
    if (is_first_word(term) && !is_path(term))
        s = autocomp_bins(term);
    else
        s = autocomp_paths(term);
    replace_word(term, s);
    string_free(s);

    return 0;
}
