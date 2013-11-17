#ifndef BRACKET_KEY_H
# define BRACKET_KEY_H

# include "terminal.h"
# include "key.h"

enum bracket_key
{
#define X(Name, Char1, Char2) Name,
#include "bracket_key.def"
#undef X
};
typedef enum bracket_key e_bracket_key;

e_next_action handle_bracket_char(s_term *term);

#endif /* !BRACKET_KEY_H */
