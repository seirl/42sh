#define _GNU_SOURCE
#include <string.h>
#include "shell_funcs.h"

static s_hashtbl *funcs_get_ptr(int reset)
{
    static s_hashtbl *funcs = NULL;
    if (reset)
        funcs = NULL;
    else if (funcs == NULL)
        funcs = hashtbl_init(hash_char,
                             cmp_char,
                             free_char,
                             ast_shell_cmd_delete);
    return funcs;
}

void funcs_set(char *name, s_ast_shell_cmd *value)
{
    hashtbl_set(funcs_get_ptr(0), value, strdup(name));
}

s_ast_shell_cmd *funcs_get(char *name)
{
    return hashtbl_get(funcs_get_ptr(0), name);
}

void funcs_unset(char *name)
{
    hashtbl_unset(funcs_get_ptr(0), name);
}

void funcs_free(void)
{
    hashtbl_free(funcs_get_ptr(0));
    funcs_get_ptr(1);
}
