#ifndef HIST_LIST_H
# define HIST_LIST_H

# include "history.h"
# include "string_utils.h"

/**
** @brief Initialize an history entry list.
*/
s_hist_list *h_list_init(void);

/**
** @brief Delete and free resources of an history entry list.
*/
void h_list_delete(s_hist_list *l);

/**
** @brief Get the nth element of an history entry list.
*/
s_hist_entry *h_list_nth(s_hist_list *l, size_t n);

/**
** @brief Add an element at the head of an history entry list.
*/
void h_list_append(s_hist_list *l, s_string *data);

#endif /* !HIST_LIST_H */
