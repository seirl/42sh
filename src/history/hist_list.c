#include "stdlib.h"
#include "string_utils.h"
#include "hist_list.h"
#include "history.h"

s_hist_list *h_list_init(void)
{
    s_hist_list *l = malloc(sizeof (s_hist_list));
    l->size = 0;
    l->hd = NULL;

    return l;
}

void h_list_delete(s_hist_list *l)
{
    s_hist_entry *this = l->hd;
    while (this)
    {
        if (this->line)
            string_free(this->line);
        if (this->temp_line)
            string_free(this->temp_line);
        l->hd = this->next;
        free(this);
        this = l->hd;
    }
    free(l);
}

void h_list_append(s_hist_list *l, s_string *data)
{
    s_hist_entry *new = malloc(sizeof (s_hist_entry));
    new->line = data;
    new->temp_line = NULL;
    new->next = l->hd;

    l->hd = new;
    l->size++;
}

s_hist_entry *h_list_nth(s_hist_list *l, size_t n)
{
    s_hist_entry *this;
    for (this = l->hd; n > 0 && this; n--)
        this = this->next;
    return this;
}
