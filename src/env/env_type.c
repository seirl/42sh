#define _GNU_SOURCE
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "env_private.h"
#include "shell_private.h"
#include "hashtbl.h"

void env_chmod(s_shell *shell, char *var, int mode)
{
    s_env_var *cur_var = hashtbl_get(shell->env, var);
    if (cur_var)
        cur_var->type = mode;
}
