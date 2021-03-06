#include "alias.h"
#include "shell_private.h"

void alias_create(s_shell *shell)
{
    shell->aliases = hashtbl_init(hash_string, cmp_string, free_string,
                                 free_string);
}

void alias_set(s_shell *shell, s_string *alias, s_string *value)
{
    hashtbl_set(shell->aliases, value, alias);
}

int alias_unset(s_shell *shell, s_string *alias)
{
    if (hashtbl_get(shell->aliases, alias) == NULL)
        return 1;
    hashtbl_unset(shell->aliases, alias);
    return 0;
}

s_string *alias_get(s_shell *shell, s_string *alias)
{
    return hashtbl_get(shell->aliases, alias);
}

void alias_free(s_shell *shell)
{
    hashtbl_free(shell->aliases);
}
