#include "terminal.h"

int is_blank(char c)
{
    return c == ' ' || c == '\t';
}

int start_word(s_term *term)
{
    if (!term->input_index)
        return 0;
    int i = term->input_index - 1;
    while (i >= 0 && !is_blank(term->input->buf[i]))
        i--;
    return i + 1;
}

size_t end_word(s_term *term)
{
    size_t i = term->input_index;
    while (term->input->buf[i] && !is_blank(term->input->buf[i]))
        i++;
    return i;
}

int is_first_word(s_term *term)
{
    int i = 0;
    while (is_blank(term->input->buf[i]))
        i++;
    return i == start_word(term);
}
