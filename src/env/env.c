#include <stdlib.h>
#include "env.h"
#include "hashtbl.h"

static s_hashtbl *env_get_ptr()
{
    static s_hashtbl *env_var = NULL;
    if (env_var == NULL)
        env_var = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    return env_var;
}

void env_set(char *name, char *value)
{
    hashtbl_set(env_get_ptr(), name, value);
}

char *env_get(char *name)
{
    return hashtbl_get(env_get_ptr(), name);
}

void env_unset(char *name)
{
    hashtbl_unset(env_get_ptr(), name);
}

void env_free()
{
    //TODO
}
