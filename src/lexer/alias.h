#ifndef ALIAS_H
# define ALIAS_H

# include "string_utils.h"
# include "hashtbl.h"

/** @brief set an alias in a hashtbl */
void alias_set(s_string *alias, s_string *value);
/** @brief remove an alias from the hashstbl */
void alias_unset(s_string *alias);
/** @brief get the value of an alias */
s_string *alias_get(s_string *alias);
/** @brief Free all the aliases */
void alias_free(void);

#endif /* !ALIAS_H */
