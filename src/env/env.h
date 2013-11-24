#ifndef ENV_H
# define ENV_H

# include "shell.h"

# define VAR_RDONLY 1

/** @brief Add a environnment variable, or change its value. */
void env_set(s_shell *shell, char *value, char *name);
/** @brief Get the value (or NULL) of an environnment variable. */
char *env_get(const s_shell *shell, char *name);
/** @brief Unset an environnment variable. */
void env_unset(s_shell *shell, char *name);
/** @brief Change a variable attributes */
void env_chmod(s_shell *shell, char *name, int mode);

#endif /* !ENV_H */
