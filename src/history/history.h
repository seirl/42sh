#ifndef HISTORY_H
# define HISTORY_H

# include "string_utils.h"

struct hist_entry
{
    s_string *line;
    s_string *temp_line;
    struct hist_entry *next;
};
typedef struct hist_entry s_hist_entry;

struct hist_list
{
    size_t size;
    s_hist_entry *hd;
};
typedef struct hist_list s_hist_list;

struct history
{
#undef lines // TODO: See if it is really needed (lines is defined en term.h)
    s_hist_list *lines;
    s_hist_entry *last_file_entry;
};
typedef struct history s_history;

/**
** @brief Query the command line history to get nth last line.
**
** @param n The number from the end of the wanted command line.
** @return an s_string representing the wanted command line.
*/
s_hist_entry *history_get(int n);
void history_open(void);
void history_close(void);

#endif /* !HISTORY_H */
