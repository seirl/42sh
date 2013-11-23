#ifndef FUNCTIONS_PRIVATE_H
# define FUNCTIONS_PRIVATE_H

# include "functions.h"
# include "ast.h"
# include "shell.h"

/** @brief Setup the shell to use functions. */
void functions_init(s_shell *shell);
/** @brief Clean the shell from functions. */
void functions_free(s_shell *shell);

#endif /* !FUNCTIONS_PRIVATE_H */
