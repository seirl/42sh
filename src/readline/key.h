#ifndef KEY_H
# define KEY_H

# include "terminal.h"

/**
** @brief What readline should do after handling the current key
*/
enum next_action
{
    CONTINUE,
    PRINT,
    RETURN,
    ERROR
};
typedef enum next_action e_next_action;

/**
** @brief Handle the next key
*/
e_next_action getkey(s_term *term, char *c);

#endif /* !KEY_H */
