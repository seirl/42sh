#include "shell_vars.h"

static s_hashtbl *vars_get_ptr(int reset)
{
    static s_hashtbl *vars = NULL;
    if (reset)
        vars = NULL;
    else if (vars == NULL)
        vars = hashtbl_init(hash_string, cmp_string, free_string, free_string);
    return vars;
}

void vars_set(s_string *name, s_string *value)
{
    hashtbl_set(vars_get_ptr(0), name, value);
}

s_string *vars_get(s_string *name)
{
    return hashtbl_get(vars_get_ptr(0), name);
}

void vars_unset(s_string *name)
{
    hashtbl_unset(vars_get_ptr(0), name);
}

void vars_free(void)
{
    hashtbl_free(vars_get_ptr(0));
    vars_get_ptr(1);
}
