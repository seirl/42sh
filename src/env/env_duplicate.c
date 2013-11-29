#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "env_private.h"
#include "shell_private.h"
#include "hashtbl.h"
#include "env.h"

static void *env_dup_value(void *value)
{
    s_env_var *val = value;
    s_env_var *var = malloc(sizeof (s_env_var));
    var->type = val->type;
    var->value = strdup(val->value);
    return var;
}

s_hashtbl *env_duplicate(s_shell *shell)
{
    s_hashtbl *h = hashtbl_duplicate(shell->env, env_dup_value, dup_char);
    return h;
}
