#ifndef SHELL_EVAL_H
# define SHELL_EVAL_H

# include "shell.h"

int shell_eval_file(s_shell *shell, FILE *f, const char *filename);

#endif /* !SHELL_EVAL_H */
