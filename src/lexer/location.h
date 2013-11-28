#ifndef LOCATION_H
# define LOCATION_H

/**
** @brief The location of a token or AST node.
*/
struct location
{
    unsigned line_start;
    unsigned line_end;
    unsigned column_start;
    unsigned column_end;
    const char *source;
};
typedef struct location s_location;

/**
** @brief Initialize location.
*/
void location_init(s_location *loc);

/**
** @brief Advance location cursor to next line.
*/
void location_next_line(s_location *loc);

/**
** @brief Advance location cursor to next column.
*/
void location_next_column(s_location *loc);

/**
** @brief Put end location to start location.
*/
void location_advance(s_location *loc);

#endif /* !LOCATION_H */
