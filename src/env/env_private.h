#ifndef ENV_PRIVATE_H
# define ENV_PRIVATE_H

# include "shell.h"

struct env_var
{
    char *value;
    int type;
};
typedef struct env_var s_env_var;

/** @brief Create the environnment. */
void env_create(s_shell *shell);
/** @brief Free the environnment. */
void env_free(s_shell *shell);

#endif /* !ENV_PRIVATE_H */
