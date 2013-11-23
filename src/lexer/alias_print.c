#include <stdio.h>
#include "alias.h"
#include "hashtbl.h"
#include "shell_private.h"

void alias_print(s_shell *shell)
{
    s_hash_elt *it = NULL;
    s_string *value;
    s_string *key;
    void *key_ptr;
    while ((value = hashtbl_iter(shell->aliases, &key_ptr, &it)) != NULL)
    {
        key = key_ptr;
        printf("%s='%s'\n", key->buf, value->buf);
    }
}
