#ifndef BRACKET_KEY_H
# define BRACKET_KEY_H

# include "terminal.h"
# include "key.h"

/**
** @brief All special keys beginning with a bracket
*/
enum bracket_key
{
# define X(Name, Char1, Char2, Fun) Name,
# include "bracket_key.def"
# undef X
};
typedef enum bracket_key e_bracket_key;

/**
** @brief Handle a special key beginning with a bracket.
*/
e_next_action handle_bracket_char(s_term *term);

#endif /* !BRACKET_KEY_H */
