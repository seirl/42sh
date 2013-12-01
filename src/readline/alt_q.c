#include "smalloc.h"
#include "terminal.h"
#include "shell.h"
#include "alt_q.h"
#include "key.h"
#include "readline.h"

static s_string_stack *alt_q_stack = NULL;

e_next_action do_alt_q(s_term *term, size_t input_index)
{
    if (!term->input->len)
        return CONTINUE;

    s_string_stack *elt = smalloc(sizeof (s_string_stack));
    elt->str = term->input;
    elt->next = alt_q_stack;
    elt->input_index = input_index;
    alt_q_stack = elt;
    term->input = string_create(0);
    term->backup_input = term->input;
    readline_update_line(term);
    return CONTINUE;
}

s_string *alt_q_pop(size_t *input_index)
{
    if (!alt_q_stack)
    {
        if (input_index)
            *input_index = 0;
        return string_create(0);
    }

    s_string_stack *head = alt_q_stack;
    s_string *ret = head->str;
    if (input_index)
        *input_index = head->input_index;
    alt_q_stack = alt_q_stack->next;
    sfree(head);
    return ret;
}

void alt_q_free(void)
{
    s_string *s = NULL;
    while ((s = alt_q_pop(NULL))->len)
        continue;
    string_free(s);
}
