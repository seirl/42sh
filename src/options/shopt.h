#ifndef SHOPT_H
# define SHOPT_H

# include "getopt.h"
# include "shell.h"

# define SHOPT_COUNT 10

//** @brief Store the state of a shell option */
struct shopt
{
    //! the shell option name
    char *name;
    //! enabled or disabled
    int value;
};
typedef struct shopt s_shopt;

/** @brief Initialize to the shell options with a default value */
void shopt_create(s_shell *shell);
/** @brief Set/unset the value of the given shell option */
int shopt_set(s_shell *shell, const char *shopt, int value);
/** @brief Get the value of the given shell option */
int shopt_get(s_shell *shell, const char *shopt);
/** @brief Print the value of all the shell options */
void shopt_print(s_shell *shell);
/** @brief Set the value according to the command line */
int shopt_from_opt(s_shell *shell, s_opt *opt);

#endif /* !SHOPT_H */
