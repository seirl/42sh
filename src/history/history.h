#ifndef HISTORY_H
# define HISTORY_H

# include "string_utils.h"
# include "list.h"

LIST(s_string *, hist_lines);
struct history
{
    s_hist_lines *lines;
    s_Node_hist_lines *last_file_entry;
};
typedef struct history s_history;

/**
** @brief Query the command line history to get nth last line.
**
** @param n The number from the end of the wanted command line.
** @return an s_string representing the wanted command line.
*/
s_string *get_history(int n);
void open_history(void);
void close_history(void);

#endif /* !HISTORY_H */
