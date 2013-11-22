#ifndef SPECIAL_KEYS_H
# define SPECIAL_KEYS_H

# include "terminal.h"
# include "key.h"
# include "bracket_key.h"

/**
** @brief A special key
*/
enum special_key
{
# define X(Name, Code1, Code2, Handler) Name,
# include "special_keys.def"
# undef X
};
typedef enum special_key e_special_key;

/**
** @brief Handle special a key
*/
e_next_action handle_special_char(s_term *term, char c);

#endif /* !SPECIAL_KEYS_H */
