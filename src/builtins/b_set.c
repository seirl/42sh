#include <stdlib.h>
#include <stdio.h>
#include "builtins.h"
#include "shell_private.h"
#include "macros.h"
#include "hashtbl.h"
#include "env_private.h"

#define UNUSED(Var) Var += 1

int builtin_set(s_shell *shell, int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    s_hash_elt *it = NULL;
    void *key;
    char *char_key;
    s_env_var *value;
    while ((value = hashtbl_iter(shell->env, &key, &it)) != NULL)
    {
        char_key = key;
        printf("%s=%s\n", char_key, value->value);
    }
    return 0;
}
