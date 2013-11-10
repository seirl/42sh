#include "location.h"

void location_next_line(s_location *loc)
{
    loc->line_end += 1;
    loc->column_end = 0;
}

void location_next_column(s_location *loc)
{
    loc->column_end += 1;
}
