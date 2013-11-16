#ifndef TERMINAL_H
# define TERMINAL_H

# include <termios.h>
# include <curses.h>
# include <term.h>

# define TERM_NAME "xterm"

struct terminal
{
    struct termios termios;
    char *bp;
    struct termios restore_state;
    char *name;
    int width;
    int height;
};
typedef struct terminal s_term;

s_term *term_get();
void term_close();

#endif /* !TERMINAL_H */
