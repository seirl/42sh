#include <stdlib.h>
#include "hashtbl.h"

s_hashtbl *hashtbl_init(unsigned long (*hash)(void *),
        int (*cmp)(void *, void *),
        void (*free_key)(void *),
        void (*free_value)(void *))
{
    s_hashtbl *h = malloc(sizeof (s_hashtbl));
    h->hash = hash;
    h->cmp = cmp;
    h->free_key = free_key;
    h->free_value = free_value;
    h->bucket = malloc(sizeof (s_hash_elt *) * HASHTBL_SIZE);
    return h;
}

void hashtbl_set(s_hashtbl *h, void *value, void *key)
{
    unsigned long hash = h->hash(key) % HASHTBL_SIZE;
    for (s_hash_elt *it = h->bucket[hash]; it; it = it->next)
    {
        if (!h->cmp(it->key, key))
        {
            h->free_value(it->value);
            it->value = value;
            return;
        }
    }
    s_hash_elt *new_elt = malloc(sizeof (s_hash_elt));
    new_elt->next = h->bucket[hash];
    new_elt->key = key;
    new_elt->value = value;
    h->bucket[hash] = new_elt;
}

void *hashtbl_get(s_hashtbl *h, void *key)
{
    unsigned long hash = h->hash(key) % HASHTBL_SIZE;
    for (s_hash_elt *it = h->bucket[hash]; it; it = it->next)
    {
        if (!h->cmp(it->key, key))
            return it->value;
    }
    return NULL;
}

void hashtbl_unset(s_hashtbl *h, void *key)
{
    unsigned long hash = h->hash(key) % HASHTBL_SIZE;
    s_hash_elt *it = NULL;
    s_hash_elt *prev = NULL;
    for (it = h->bucket[hash]; it; it = it->next)
    {
        if (!h->cmp(it->key, key))
            break;
        prev = it;
    }
    if (it)
    {
        if (prev)
            prev->next = it->next;
        else
            h->bucket[hash] = it->next;
    }
}
