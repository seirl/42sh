#ifndef ALIAS_H
# define ALIAS_H

# include "string_utils.h"
# include "hashtbl.h"
# include "shell.h"

/** @brief set an alias in a hashtbl */
void alias_create(s_shell *shell);
/** @brief set an alias in a hashtbl */
void alias_set(s_shell *shell, s_string *alias, s_string *value);
/** @brief remove an alias from the hashstbl */
int alias_unset(s_shell *shell, s_string *alias);
/** @brief get the value of an alias */
s_string *alias_get(s_shell *shell, s_string *alias);
/** @brief Free all the aliases */
void alias_free(s_shell *shell);
/** @brief Print all the alias */
void alias_print(s_shell *shell);
/** @brief Remove all the alias entries */
void alias_clear(s_shell *shell);

#endif /* !ALIAS_H */
