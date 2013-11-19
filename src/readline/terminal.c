#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "terminal.h"
#include "env.h"
#include "smalloc.h"

static s_term *g_term = NULL;

static void term_init(void)
{
    g_term = smalloc(sizeof (struct terminal));
    if (tcgetattr(STDIN_FILENO, &g_term->restore_state) != 0)
    {
        sfree(g_term);
        g_term = NULL;
    }
    g_term->termios = g_term->restore_state;
    g_term->name = env_get("TERM");
    g_term->bp = smalloc(2048);
    if (tgetent(g_term->bp, g_term->name) <= 0)
        tgetent(g_term->bp, "xterm");
    g_term->termios.c_iflag &= ~ICRNL;
    g_term->termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    g_term->termios.c_cc[VMIN] = 1;
    g_term->termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &g_term->termios);
    g_term->input = string_create(0);
    g_term->input_index = 0;
    g_term->hist_current = NULL;
    g_term->hist_pos = 0;
    g_term->prompt = NULL;
}

s_term *term_get(void)
{
    if (!g_term)
        term_init();

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
