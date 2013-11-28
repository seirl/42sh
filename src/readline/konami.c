#include <string.h>
#include "konami.h"
#include "terminal.h"
#include "shell.h"
#include "readline.h"

static const e_konami_key konami_sequence[] =
{
    KONAMI_UP,
    KONAMI_UP,
    KONAMI_DOWN,
    KONAMI_DOWN,
    KONAMI_LEFT,
    KONAMI_RIGHT,
    KONAMI_LEFT,
    KONAMI_RIGHT,
    KONAMI_B,
    KONAMI_A,
};

static int konami_done(s_term *term)
{
    if (strcmp(term->input->buf, "ba"))
        return 0;

    term->input_index--;
    string_reset(term->input);
    string_puts(term->input, "Votai Test.");
    readline_update_line(term);
    term->input_index = term->input->len;

    return 1;
}

int konami_next(s_term *term, e_konami_key key)
{
    static int konami_state = 0;
    if (key == konami_sequence[konami_state])
        konami_state++;
    else
        konami_state = 0;

    if (konami_state == 10)
    {
        konami_state = 0;
        return konami_done(term);
    }
    return 0;
}
