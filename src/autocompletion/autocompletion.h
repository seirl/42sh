#ifndef AUTOCOMPLETION_H
# define AUTOCOMPLETION_H

# include "terminal.h"

/**
** @brief autocomplete the current word on the terminal.
*/
int autocomplete(s_shell *shell, s_term *term);

/**
** @brief Reconstruct the internal representation of paths.
*/
void rehash(s_shell *shell);

/**
** @brief Free the internal representation of paths.
*/
void rehash_free(void);

#endif /* !AUTOCOMPLETION_H */
