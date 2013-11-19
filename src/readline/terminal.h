#ifndef TERMINAL_H
# define TERMINAL_H

# include <termios.h>
# include <curses.h>
# include <term.h>
# include "string_utils.h"
# include "history.h"

# define TERM_NAME "xterm"

/**
** @brief Terminal informations
*/
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
    s_hist_entry *hist_current;
    size_t hist_pos;
};
typedef struct terminal s_term;

/**
** @brief Get terminal informations
*/
s_term *term_get(void);

/**
** @brief Close and free the resources of a terminal informations structure
*/
void term_close(void);

#endif /* !TERMINAL_H */
