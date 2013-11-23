#ifndef ENV_PRIVATE_H
# define ENV_PRIVATE_H

# include "shell.h"

/** @brief Create the environnment. */
void env_create(s_shell *shell);
/** @brief Free the environnment. */
void env_free(s_shell *shell);

#endif /* !ENV_PRIVATE_H */
