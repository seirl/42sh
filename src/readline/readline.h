#ifndef READLINE_H
# define READLINE_H

# include "string_utils.h"

/**
** @brief read an input line providing editing capabilities.
**
** @arg prompt The prompt displayed to the user.
** @return NULL if a fatal error occured, the input line otherwise.
*/
s_string *readline(char *prompt);

#endif /* !READLINE_H */
