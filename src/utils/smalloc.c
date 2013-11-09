#include "smalloc.h"

static u_smalloc_bucket **get_bucket()
{
    static u_smalloc_bucket **buckets = NULL;
    if (buckets == NULL)
        buckets = calloc(N_BUCKETS, sizeof (u_smalloc_bucket*));
    return buckets;
}

static size_t ptr_hash(void *ptr)
{
    char *ptr_char = ptr;
    char *ptr_null = NULL;
    return ((ptr_char - ptr_null) / 4) % N_BUCKETS;
}

static u_smalloc_bucket *get_remove(u_smalloc_bucket *meta)
{
    size_t hash = ptr_hash(meta);
    u_smalloc_bucket *tmp = get_bucket()[hash];
    u_smalloc_bucket *prev = NULL;
    while (tmp != NULL)
    {
        if (tmp == meta)
        {
            if (prev)
                prev->next = meta->next;
            else
                get_bucket()[hash] = meta->next;
            return meta;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void *smalloc(size_t size)
{
    void *ptr = malloc(size + sizeof (u_smalloc_bucket));
    size_t hash = ptr_hash(ptr);
    u_smalloc_bucket **buckets = get_bucket();
    u_smalloc_bucket *node = ptr;
    node->next = buckets[hash];
    buckets[hash] = node;

    char *char_ptr = ptr;
    return char_ptr + sizeof (u_smalloc_bucket);
}

void *scalloc(size_t nmemb, size_t size)
{
    char *ret = smalloc(nmemb * size);
    for (size_t i = 0; i < nmemb * size; ++i)
        ret[i] = 0;
    return ret;
}

void *srealloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return smalloc(size);
    if (size == 0)
    {
        sfree(ptr);
        return NULL;
    }
    char *char_ptr = ptr;
    char_ptr -= sizeof (u_smalloc_bucket);
    ptr = char_ptr;
    u_smalloc_bucket *meta = ptr;
    u_smalloc_bucket *new_ptr = realloc(meta, size +
            sizeof (u_smalloc_bucket));
    if (new_ptr != meta)
    {
        get_remove(meta);
        int hash = ptr_hash(new_ptr);
        u_smalloc_bucket **buckets = get_bucket();
        new_ptr->next = buckets[hash];
        buckets[hash] = new_ptr;
        return new_ptr + sizeof (u_smalloc_bucket);
    }
    return ptr;
}

void sfree(void *ptr)
{
    char *char_ptr = ptr;
    ptr = char_ptr - sizeof (u_smalloc_bucket);
    u_smalloc_bucket *meta = ptr;
    free(get_remove(meta));
}

void smalloc_clean()
{
    u_smalloc_bucket *tmp;
    u_smalloc_bucket **buckets = get_bucket();
    for (int i = 0; i < N_BUCKETS; ++i)
    {
        while (buckets[i] != NULL)
        {
            tmp = buckets[i]->next;
            free(buckets[i]);
            buckets[i] = tmp;
        }
    }
    free(buckets);
}
