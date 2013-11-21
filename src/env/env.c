#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>

#include "shell_private.h"
#include "env.h"
#include "hashtbl.h"

void env_set(s_shell *shell, char *value, char *name)
{
    hashtbl_set(shell->env, strdup(value), strdup(name));
}

char *env_get(s_shell *shell, char *name)
{
    return hashtbl_get(shell->env, name);
}

void env_unset(s_shell *shell, char *name)
{
    hashtbl_unset(shell->env, name);
}

void env_free(s_shell *shell)
{
    hashtbl_free(shell->env);
}
