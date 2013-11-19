#include "alias.h"

HASHTBL(s_string*, s_string*, alias_table);
HASHTBL_FUN(alias_table, null_free, null_free)

static s_alias_table *alias_get_ptr()
{
    static s_alias_table *aliases = NULL;
    if (aliases == NULL)
        HASHTBL_INIT(aliases, 42, hash_string, cmp_string, free_alias_table);
    return aliases;
}

void alias_set(s_string *alias, s_string *value)
{
    HASHTBL_SET(alias_get_ptr(), value, alias);
}

void alias_unset(s_string *alias)
{
    HASHTBL_DEL(alias_get_ptr(), alias);
}

s_string *alias_get(s_string *alias)
{
    s_string *ret;
    int changed;
    HASHTBL_GET(alias_get_ptr(), alias, ret, changed);
    if (changed)
        return ret;
    return NULL;
}

void alias_free()
{
    HASHTBL_FREE(alias_get_ptr());
}
