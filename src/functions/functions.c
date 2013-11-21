#include <string.h>

#include "functions.h"
#include "shell_private.h"

static void functions_delete_entry(void *shell_cmd)
{
    s_ast_shell_cmd *node = shell_cmd;
    ast_shell_cmd_delete(node);
}

void functions_init(s_shell *shell)
{
    shell->functions = hashtbl_init(hash_char,
                                    cmp_char,
                                    free_char,
                                    functions_delete_entry);
}

void functions_set(s_shell *shell, char *name, s_ast_shell_cmd *value)
{
    hashtbl_set(shell->functions, value, strdup(name));
}

s_ast_shell_cmd *functions_get(s_shell *shell, char *name)
{
    return hashtbl_get(shell->functions, name);
}

void functions_unset(s_shell *shell, char *name)
{
    hashtbl_unset(shell->functions, name);
}

void functions_free(s_shell *shell)
{
    hashtbl_free(shell->functions);
    shell->functions = NULL;
}

