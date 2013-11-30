#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "alias.h"
#include "hashtbl.h"
#include "shell_private.h"
#include "smalloc.h"

struct alias_sort
{
    char *key;
    char *value;
};
typedef struct alias_sort s_alias_sort;

static int cmp_alias(const void *p1, const void *p2)
{
    s_alias_sort *const *a1 = p1;
    s_alias_sort *const *a2 = p2;
    return strcmp((*a1)->key, (*a2)->key);
}

static s_alias_sort **alias_push(s_alias_sort **aliases, char *value,
                                 char *key, int *n)
{
    *n += 1;
    aliases = srealloc(aliases, sizeof (char *) * (*n));
    s_alias_sort *new_alias = smalloc(sizeof (s_alias_sort));
    new_alias->key = key;
    new_alias->value = value;
    aliases[*n - 1] = new_alias;
    return aliases;
}

void alias_print(s_shell *shell)
{
    s_hash_elt *it = NULL;
    s_string *value;
    s_string *key;
    void *key_ptr;
    int len = 0;
    s_alias_sort **array = NULL;
    while ((value = hashtbl_iter(shell->aliases, &key_ptr, &it)) != NULL)
    {
        key = key_ptr;
        array = alias_push(array, value->buf, key->buf, &len);
    }
    qsort(array, len, sizeof (s_alias_sort *), cmp_alias);
    for (int i = 0; i < len; ++i)
    {
        printf("%s='%s'\n", array[i]->key, array[i]->value);
        sfree(array[i]);
    }
    sfree(array);
}
