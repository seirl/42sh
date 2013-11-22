#ifndef SHOPT_H
# define SHOPT_H

# include "getopt.h"

//** @brief Store the state of a shell option */
struct shopt
{
    //! the shell option name
    char *name;
    //! enabled or disabled
    int value;
};
typedef struct shopt s_shopt;

/** @brief Set/unset the value of the given shell option */
int shopt_set(const char *shopt, int value);
/** @brief Get the value of the given shell option */
int shopt_get(const char *shopt);
/** @brief Print the value of all the shell options */
void shopt_print(void);
/** @brief Set the value according to the command line */
int shopt_from_opt(s_opt *opt);
//* @brief Set the environnment variable SHELLOPTS*/
void shopt_update_env();

#endif /* !SHOPT_H */
