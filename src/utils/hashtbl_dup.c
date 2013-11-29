#include <stdlib.h>
#include "hashtbl.h"

s_hashtbl *hashtbl_duplicate(s_hashtbl *h, void *(*value_dup)(void *),
                             void *(*key_dup)(void *))
{
    s_hashtbl *new_h = hashtbl_init(h->hash, h->cmp, h->free_key,
                                    h->free_value);
    for (int i = 0; i < HASHTBL_SIZE; ++i)
    {
        s_hash_elt *it = h->bucket[i];
        s_hash_elt *prev = new_h->bucket[i];
        while (it)
        {
            s_hash_elt *elt = malloc(sizeof (s_hash_elt));
            elt->next = NULL;
            elt->value = value_dup(it->value);
            elt->key = key_dup(it->key);
            prev->next = elt;
            prev = elt;
            it = it->next;
        }
    }
    return new_h;
}
