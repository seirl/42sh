#ifndef OPTIONS_H
# define OPTIONS_H

# include "shell.h"

enum option_return
{
    E_RET   = 1,
    E_ERROR = 2,
    E_FILE  = 4,
    E_STR   = 8,
    E_INTE  = 16,
    E_RC    = 32
};
typedef enum option_return e_option_return;

/** @brief Parse the command line and handle the options */
e_option_return parse_options(s_shell *shell, int argc, char *argv[],
                             char **source);

#endif /* !OPTIONS_H */
