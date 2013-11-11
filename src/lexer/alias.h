#ifndef ALIAS_H
# define ALIAS_H

# include "string_utils.h"
# include "hashtbl.h"

void alias_set(s_string *alias, s_string *value);
void alias_unset(s_string *alias);
s_string *alias_get(s_string *alias);
void alias_free();

#endif /* !ALIAS_H */
