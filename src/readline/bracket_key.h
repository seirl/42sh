#ifndef BRACKET_KEY_H
# define BRACKET_KEY_H

# include "shell.h"
# include "terminal.h"
# include "key.h"
# include "special_keys.h"

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
** @brief Handle a special control sequence beginning with a bracket.
*/
e_next_action handle_bracket_char(s_shell *shell, s_term *term);

/**
** @brief Handle a particular special key (of bracket type)
*/
e_next_action handle_bracket_key(s_shell *shell, e_bracket_key key,
                                 s_term *term);

#endif /* !BRACKET_KEY_H */
