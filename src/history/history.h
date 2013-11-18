#ifndef HISTORY_H
# define HISTORY_H

# include "string_utils.h"

/**
** @brief Query the command line history to get nth last line.
**
** @param n The number from the end of the wanted command line.
** @return an s_string representing the wanted command line.
*/
s_string *get_history(size_t n);

#endif /* !HISTORY_H */
