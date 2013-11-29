#ifndef AUTOCOMPLETION_H
# define AUTOCOMPLETION_H

# include "terminal.h"

struct autocomplete_binaries
{
    char ***bins;
    size_t size;
    size_t max_size;
};
typedef struct autocomplete_binaries s_autocomplete_binaries;

/**
** @brief autocomplete the current word on the terminal.
*/
int autocomplete(s_shell *shell, s_term *term);

/**
** @brief Reconstruct the internal representation of paths.
*/
void rehash(s_shell *shell);

#endif /* !AUTOCOMPLETION_H */
