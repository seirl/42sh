#ifndef ESCAPE_KEY_H
# define ESCAPE_KEY_H

# include "terminal.h"
# include "key.h"
# include "shell.h"

/**
** @brief All special keys beginning with a bracket
*/
enum escape_key
{
# define X(Name, Char1, Char2, Fun) Name,
# include "escape_keys.def"
# undef X
};
typedef enum escape_key e_escape_key;

/**
** @brief Handle a special control sequence beginning with an escape character.
*/
e_next_action handle_escape_char(s_shell *shell, s_term *term, char c);

/**
** @brief Handle a particular special key (of escape type)
*/
e_next_action handle_escape_key(s_shell *shell, s_term *term,
                                e_escape_key key);

#endif /* !ESCAPE_KEY_H */
