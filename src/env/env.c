#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "env_private.h"

#include "shell_private.h"
#include "hashtbl.h"

void env_create(s_shell *shell)
{
    assert(!shell->env);
    shell->env = hashtbl_init(hash_char, cmp_char, free_char, free_char);
}

void env_set(s_shell *shell, char *value, char *name)
{
    hashtbl_set(shell->env, strdup(value), strdup(name));
}

char *env_get(const s_shell *shell, char *name)
{
    return hashtbl_get(shell->env, name);
}

void env_unset(s_shell *shell, char *name)
{
    hashtbl_unset(shell->env, name);
}

void env_free(s_shell *shell)
{
    assert(shell->env);
    hashtbl_free(shell->env);
    shell->env = NULL;
}
