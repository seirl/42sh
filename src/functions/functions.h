#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "functions.h"
# include "ast.h"
# include "shell.h"

//! @brief Setup the shell to use functions. */
void functions_init(s_shell *shell);
//! @brief Add a function. */
void functions_set(s_shell *shell, char *name, s_ast_shell_cmd *value);
//! @brief Get a function. */
s_ast_shell_cmd *functions_get(s_shell *shell, char *name);
//! @brief Remove a function. */
void functions_unset(s_shell *shell, char *name);
//! @brief Clean the shell from functions. */
void functions_free(s_shell *shell);

#endif /* !FUNCTIONS_H */
