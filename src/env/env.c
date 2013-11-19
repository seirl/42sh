#include <stdlib.h>
#include "env.h"
#include "hashtbl.h"

HASHTBL(char*, char*, env);
HASHTBL_FUN(env, null_free, null_free)

static s_env *env_get_ptr()
{
    static s_env *env_var = NULL;
    if (env_var == NULL)
        HASHTBL_INIT(env_var, 100, hash_char, cmp_char, free_env);
    return env_var;
}

void env_set(char *name, char *value)
{
    HASHTBL_SET(env_get_ptr(), name, value);
}

char *env_get(char *name)
{
    int changed = 0;
    char *ret;
    HASHTBL_GET(env_get_ptr(), name, ret, changed);
    if (changed)
        return ret;
    return NULL;
}

void env_unset(char *name)
{
    HASHTBL_DEL(env_get_ptr(), name);
}

void env_free()
{
    HASHTBL_FREE(env_get_ptr());
}
