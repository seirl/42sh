#include "wrapper.h"
#include "cursor_handler.h"

void curs_left(s_term *term)
{
    my_tputs(tgetstr("le", &term->bp));
}

void curs_right(s_term *term)
{
    my_tputs(tgetstr("nd", &term->bp));
}

void curs_up(s_term *term)
{
    my_tputs(tgetstr("up", &term->bp));
}

void curs_down(s_term *term)
{
    my_tputs(tgetstr("do", &term->bp));
}
