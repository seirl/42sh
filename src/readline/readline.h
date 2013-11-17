#ifndef READLINE_H
# define READLINE_H

# include "string_utils.h"

/**
** @brief read an input line providing editing capabilities.
**
** @return NULL if a fatal error occured, the input line otherwise.
*/
s_string *readline();

#endif /* !READLINE_H */
