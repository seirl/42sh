#ifndef TERMINAL_H
# define TERMINAL_H

# include <termios.h>
# include <curses.h>
# include <term.h>

# include "shell.h"
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
    s_string *backup_input;
    size_t input_index;
    s_hist_entry *hist_current;
    int hist_pos;
    char *prompt;
};
typedef struct terminal s_term;

/**
** @brief Tell if the character at 'index' position on the input is a blank.
*/
int input_is_blank(s_term *term, size_t index);

/**
** @brief Get terminal informations
*/
s_term *term_get(s_shell *shell);

/**
** @brief Close and free the resources of a terminal informations structure
*/
void term_close(void);

#endif /* !TERMINAL_H */
