#ifndef KEY_H
# define KEY_H

# include "terminal.h"

enum next_action
{
    CONTINUE,
    RETURN,
    ERROR
};
typedef enum next_action e_next_action;

e_next_action getkey(s_term *term, char *c);

#endif /* !KEY_H */
