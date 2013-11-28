#ifndef PROMPT_H
# define PROMPT_H

# include "env.h"
# include "string_utils.h"

/**
** @brief Expand variables in (like \a, \D{format} etc) in a prompt.
*/
void prompt_expand(s_shell *shell, s_string **prompt);

#endif /* !PROMPT_H */
