#ifndef ALT_Q_H
# define ALT_Q_H

# include "terminal.h"
# include "key.h"
# include "shell.h"

struct string_stack
{
    s_string *str;
    struct string_stack *next;
};
typedef struct string_stack s_string_stack;

/**
** @brief Handle the alt_q key sequence.
*/
e_next_action do_alt_q(s_term *term);

/**
** @brief Get the top of the alt_q stack.
*/
s_string *alt_q_pop(void);

/**
** @brief Free the internal stack of alt_q.
*/
void alt_q_free(void);

#endif /* !ALT_Q_H */
