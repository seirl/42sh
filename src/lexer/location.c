#include "location.h"

void location_init(s_location *loc)
{
    loc->line_start = 0;
    loc->line_end = 0;
    loc->column_start = 0;
    loc->column_end = 0;
    loc->source = "";
}

void location_next_line(s_location *loc)
{
    loc->line_end += 1;
    loc->column_end = 0;
}

void location_next_column(s_location *loc)
{
    loc->column_end += 1;
}

void location_advance(s_location *loc)
{
    loc->line_start = loc->line_end;
    loc->column_start = loc->column_end;
}
