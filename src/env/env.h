#ifndef ENV_H
# define ENV_H

# include "shell.h"

/** @brief Add a environnment variable, or change its value. */
void env_set(s_shell *shell, char *value, char *name);
/** @brief Get the value (or NULL) of an environnment variable. */
char *env_get(s_shell *shell, char *name);
/** @brief Unset an environnment variable. */
void env_unset(s_shell *shell, char *name);
/** @brief Free the environnment (unset all the variables). */
void env_free(s_shell *shell);

#endif /* !ENV_H */
