#ifndef KEY_H
# define KEY_H

# include "shell.h"
# include "terminal.h"

/**
** @brief What readline should do after handling the current key
*/
enum next_action
{
    CONTINUE,
    PRINT,
    RETURN,
    ERROR,
    EOI
};
typedef enum next_action e_next_action;

/**
** @brief Handle the given key
*/
e_next_action handle_key(s_shell *shell, s_term *term, char *c);

/**
** @brief Handle the next key
*/
e_next_action getkey(s_shell *shell, s_term *term, char *c);

#endif /* !KEY_H */
