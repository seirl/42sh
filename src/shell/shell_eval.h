#ifndef SHELL_EVAL_H
# define SHELL_EVAL_H

# include <stdio.h>
# include "shell.h"

int shell_eval_file(s_shell *shell, FILE *f, const char *filename);
int shell_eval_str(s_shell *shell, const char *str);

#endif /* !SHELL_EVAL_H */
