#ifndef KONAMI_H
# define KONAMI_H

# include "shell.h"
# include "terminal.h"

enum konami_key
{
    KONAMI_UP,
    KONAMI_DOWN,
    KONAMI_LEFT,
    KONAMI_RIGHT,
    KONAMI_A,
    KONAMI_B,
    KONAMI_NOTHING,
};
typedef enum konami_key e_konami_key;

/**
** @brief Inform the konami module that the given key was typed
** @return An integer telling if the konami sequence was completed
*/
int konami_next(s_term *term, e_konami_key key);

#endif /* !KONAMI_H */
