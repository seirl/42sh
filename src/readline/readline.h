#ifndef READLINE_H
# define READLINE_H

# include "shell.h"
# include "string_utils.h"
# include "terminal.h"

/**
** @brief read an input line providing editing capabilities.
**
** @arg prompt The prompt displayed to the user.
** @return NULL if a fatal error occured, the input line otherwise.
*/
s_string *readline(s_shell *shell, char *prompt);

/**
** @brief Print the given character on the correct place of the line (according
** to cursor position).
*/
void readline_do_print(s_shell *shell, char c);

/**
** @brief Update the terminal line according to the content of term->input.
*/
void readline_update_line(s_term *term);

#endif /* !READLINE_H */
