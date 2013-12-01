#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "env.h"
#include "smalloc.h"
#include "alt_q.h"

static s_term *get_term_ptr(int set, s_term *new_term)
{
    static s_term *term = NULL;
    if (set)
        term = new_term;
    return term;
}

static void term_init(s_shell *shell)
{
    get_term_ptr(1, smalloc(sizeof (struct terminal)));
    s_term *term = get_term_ptr(0, NULL);
    if (tcgetattr(STDIN_FILENO, &term->restore_state) != 0)
    {
        sfree(term);
        term = NULL;
    }
    term->termios = term->restore_state;
    term->name = env_get(shell, "TERM");
    term->bp = smalloc(2048);
    if (tgetent(term->bp, term->name) <= 0)
        tgetent(term->bp, "xterm");
    term->termios.c_iflag &= ~ICRNL;
    term->termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    term->termios.c_cc[VMIN] = 1;
    term->termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &term->termios);
    term->backup_input = alt_q_pop(&term->input_index);
    term->input = term->backup_input;
    term->hist_current = NULL;
    term->hist_pos = -1;
    term->prompt = NULL;
}

int input_is_blank(s_term *term, size_t index)
{
    char c = term->input->buf[index];
    return (c == ' ') || (c == '\t');
}

s_term *term_get(s_shell *shell)
{
    if (!get_term_ptr(0, NULL))
        term_init(shell);

    return get_term_ptr(0, NULL);
}

void term_close(void)
{
    s_term *term = get_term_ptr(0, NULL);
    if (!term)
        return;
    sfree(term->bp);
    string_free(term->backup_input);
    tcsetattr(STDIN_FILENO, TCSANOW, &(term->restore_state));
    sfree(term);
    get_term_ptr(1, NULL);
}
