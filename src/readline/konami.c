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

static void konami_done(s_term *term)
{
    if (strcmp(term->input->buf, "ba"))
        return;

    string_reset(term->input);
    string_puts(term->input, " Votai Test.");
    readline_update_line(term);
    term->input_index = term->input->len;
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
        konami_done(term);
        konami_state = 0;
        return 1;
    }
    return 0;
}
