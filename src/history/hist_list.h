#ifndef HIST_LIST_H
# define HIST_LIST_H

# include "history.h"
# include "string_utils.h"

s_hist_list *h_list_init(void);
void h_list_delete(s_hist_list *l);
s_hist_entry *h_list_nth(s_hist_list *l, size_t n);
void h_list_append(s_hist_list *l, s_string *data);

#endif /* !HIST_LIST_H */
