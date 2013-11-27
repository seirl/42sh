#ifndef HISTORY_H
# define HISTORY_H

# include <time.h>
# include "shell.h"
# include "string_utils.h"

/**
** @brief History entry
*/
struct hist_entry
{
    s_string *line;
    s_string *temp_line;
    time_t date;
    struct hist_entry *next;
};
typedef struct hist_entry s_hist_entry;

/**
** @brief History entry list
*/
struct hist_list
{
    int size;
    s_hist_entry *hd;
};
typedef struct hist_list s_hist_list;

/**
** @brief History
*/
struct history
{
// TODO: See if it is really needed (lines is defined en term.h)
# undef lines
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
s_hist_entry *history_get(s_shell *shell, int n);

/**
** @brief Close the history and free it's ressources
*/
void history_close(s_shell *shell);

/**
** @brief Remove every entry of the local history.
*/
void history_reset(s_shell *shell);

/**
** @brief Get the history size
*/
int history_size(s_shell *shell);

/**
** @brief Add a line to the history
*/
void history_add(s_shell *shell, s_string *line);

#endif /* !HISTORY_H */
