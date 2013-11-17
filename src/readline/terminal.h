#ifndef TERMINAL_H
# define TERMINAL_H

# include <termios.h>
# include <curses.h>
# include <term.h>
# include "string_utils.h"

# define TERM_NAME "xterm"

struct terminal
{
    struct termios termios;
    char *bp;
    struct termios restore_state;
    char *name;
    struct
    {
        int x;
        int y;
    } cursor;
    int width;
    int height;
    s_string *input;
    size_t input_index;
};
typedef struct terminal s_term;

s_term *term_get();
void term_close();

#endif /* !TERMINAL_H */
