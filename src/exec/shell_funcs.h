#ifndef SHELL_FUNCS_H
# define SHELL_FUNCS_H

# include "hashtbl.h"
# include "ast.h"

void funcs_set(char *name, s_ast_shell_cmd *value);
s_ast_shell_cmd *funcs_get(char *name);
void funcs_unset(char *name);
void funcs_free(void);

#endif /* !SHELL_FUNCS_H */
