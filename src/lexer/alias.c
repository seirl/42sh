#include "alias.h"

static s_hashtbl *alias_get_ptr()
{
    static s_hashtbl *aliases = NULL;
    if (aliases == NULL)
        aliases = hashtbl_init(hash_string, cmp_string, free_string, free_string);
    return aliases;
}

void alias_set(s_string *alias, s_string *value)
{
    hashtbl_set(alias_get_ptr(), value, alias);
}

void alias_unset(s_string *alias)
{
    hashtbl_unset(alias_get_ptr(), alias);
}

s_string *alias_get(s_string *alias)
{
    return hashtbl_get(alias_get_ptr(), alias);
}

void alias_free()
{
    hashtbl_free(alias_get_ptr());
}
