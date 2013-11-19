#include "shell_funcs.h"

static s_hashtbl *funcs_get_ptr(void)
{
    static s_hashtbl *funcs = NULL;
    if (funcs == NULL)
        funcs = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    return funcs;
}

void funcs_set(char *name, s_ast_shell_cmd *value)
{
    hashtbl_set(funcs_get_ptr(), name, value);
}

s_ast_shell_cmd *funcs_get(char *name)
{
    return hashtbl_get(funcs_get_ptr(), name);
}

void funcs_unset(char *name)
{
    hashtbl_unset(funcs_get_ptr(), name);
}

void funcs_free(void)
{
    hashtbl_free(funcs_get_ptr());
}
