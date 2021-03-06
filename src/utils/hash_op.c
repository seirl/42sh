#include <stdlib.h>
#include "hashtbl.h"

static s_hash_elt *next_bucket(s_hashtbl *h, s_hash_elt *current)
{
    unsigned long bucket;
    if (current == NULL)
        bucket = 0;
    else
        bucket = h->hash(current->key) % HASHTBL_SIZE + 1;
    while (bucket < HASHTBL_SIZE - 1 && h->bucket[bucket] == NULL)
        ++bucket;
    if (bucket == HASHTBL_SIZE)
        return NULL;
    return h->bucket[bucket];
}

static s_hash_elt *find_next(s_hashtbl *h, s_hash_elt *current)
{
    if (current->next)
        return current->next;
    return next_bucket(h, current);
}

void *hashtbl_iter(s_hashtbl *h, void **key, s_hash_elt **it)
{
    if (it == NULL || *it == NULL)
        *it = next_bucket(h, NULL);
    else
        *it = find_next(h, *it);

    if (*it)
    {
        *key = (*it)->key;
        return (*it)->value;
    }
    *key = NULL;
    return NULL;
}
