#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "terminal.h"
#include "smalloc.h"

static s_term *term_init()
{
    s_term *term = smalloc(sizeof (struct terminal));
    if (tcgetattr(STDIN_FILENO, &term->restore_state) != 0)
    {
        sfree(term);
        return NULL;
    }
    term->name = TERM_NAME; //TODO var_get("TERM");
    term->bp = smalloc(2048);
    if (tgetent(term->bp, term->name) <= 0)
    {
        sfree(term);
        sfree(term->bp);
        return NULL;
    }
    term->termios.c_lflag &= ~(ICANON | ECHO);
    term->termios.c_cc[VMIN] = 1;
    term->termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &term->termios);
    return term;
}

s_term *term_get()
{
    static s_term *term = NULL;
    if (term == NULL)
    {
        term = term_init();
        //TODO handle errors
    }
    return term;
}

void term_close()
{
    s_term *term = term_get();
    //sfree(term->bp); //FIXME ????
    tcsetattr(STDIN_FILENO, TCSANOW, &term->restore_state);
    sfree(term);
}
