#ifndef SHELL_VARS_H
# define SHELL_VARS_H

# include "hashtbl.h"
# include "ast.h"

void vars_set(s_string *name, s_string *value);
s_string *vars_get(s_string *name);
void vars_unset(s_string *name);
void vars_free(void);

#endif /* !SHELL_VARS_H */
