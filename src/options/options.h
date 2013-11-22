#ifndef OPTIONS_H
# define OPTIONS_H

# include "shell.h"

/** @brief Parse the command line and handle the options */
int parse_options(s_shell *shell, int argc, char *argv[], char **cmd, char **file);

#endif /* !OPTIONS_H */
