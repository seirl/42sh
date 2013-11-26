#ifndef READLINE_H
# define READLINE_H

# include "shell.h"
# include "string_utils.h"

/**
** @brief read an input line providing editing capabilities.
**
** @arg prompt The prompt displayed to the user.
** @return NULL if a fatal error occured, the input line otherwise.
*/
s_string *readline(s_shell *shell, char *prompt);
void do_print(s_shell *shell, char c);

#endif /* !READLINE_H */
