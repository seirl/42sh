#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "env_private.h"
#include "shell_private.h"
#include "hashtbl.h"

static void free_env_var(void *var)
{
    s_env_var *v = var;
    free(v->value);
    free(v);
}

void env_create(s_shell *shell)
{
    assert(!shell->env);
    shell->env = hashtbl_init(hash_char, cmp_char, free_char, free_env_var);
}

void env_set(s_shell *shell, char *value, char *name)
{
    s_env_var *new_var = malloc(sizeof (s_env_var));
    new_var->type = 0;
    new_var->value = strdup(value);
    hashtbl_set(shell->env, new_var, strdup(name));
}

char *env_get(const s_shell *shell, char *name)
{
    s_env_var *var = hashtbl_get(shell->env, name);
    return var ? var->value : NULL;
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
