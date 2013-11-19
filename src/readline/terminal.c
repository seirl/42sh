#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "terminal.h"
#include "env.h"
#include "smalloc.h"

static s_term *g_term = NULL;

static s_term *term_init(void)
{
    s_term *term = smalloc(sizeof (struct terminal));
    if (tcgetattr(STDIN_FILENO, &term->restore_state) != 0)
    {
        sfree(term);
        return NULL;
    }
    term->termios = term->restore_state;
    term->name = env_get("TERM");
    term->bp = smalloc(2048);
    if (tgetent(term->bp, term->name) <= 0)
        tgetent(term->bp, "xterm");
    term->termios.c_iflag &= ~ICRNL;
    term->termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    term->termios.c_cc[VMIN] = 1;
    term->termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &term->termios);
    term->input = string_create(0);
    term->input_index = 0;
    term->hist_current = NULL;
    term->hist_pos = 0;
    return term;
}

s_term *term_get(void)
{
    if (g_term == NULL)
        g_term = term_init();

    return g_term;
}

void term_close(void)
{
    if (!g_term)
        return;
    sfree(g_term->bp);
    string_free(g_term->input);
    tcsetattr(STDIN_FILENO, TCSANOW, &(g_term->restore_state));
    sfree(g_term);
    g_term = NULL;
}
