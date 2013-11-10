#ifndef HASHTBL_H
# define HASHTBL_H

# include "list.h"

# define HASHTBL(Type, Keytype, Name)                                  \
    struct item_##Name                                                 \
    {                                                                  \
        Keytype key;                                                   \
        Type value;                                                    \
    };                                                                 \
    typedef struct item_##Name s_item_##Name;                          \
    LIST(s_item_##Name, items_##Name);                                 \
    struct Name                                                        \
    {                                                                  \
        s_items_##Name **bucket;                                       \
        size_t size;                                                   \
        int(*hash)(Keytype);                                           \
        int(*cmp)(Keytype, Keytype);                                   \
        s_item_##Name *it;                                             \
    };                                                                 \
    typedef struct Name s_##Name;

# define HASHTBL_INIT(Table, Size, Hash, Cmp, F)                       \
    do {                                                               \
        size_t i;                                                      \
        Table = malloc(sizeof (*Table));                               \
        Table->hash = Hash;                                            \
        Table->cmp = Cmp;                                              \
        Table->size = Size;                                            \
        Table->bucket = malloc(sizeof (*Table->bucket) * Size);        \
        for (i = 0; i < Size; ++i)                                     \
            LIST_INIT(Table->bucket[i], F);                            \
    } while(0)

# define HASHTBL_SET(Table, Value, Key)                                \
    do {                                                               \
        int index = Table->hash(Key);                                  \
        Table->it = malloc(sizeof (*Table->it));                       \
        Table->it->key = Key;                                          \
        Table->it->value = Value;                                      \
        LIST_INSERT_HEAD(Table->bucket[index], *Table->it);            \
        free(Table->it);                                               \
    } while(0)

# define HASHTBL_GET(Table, Key, Ret, Changed)                         \
    do {                                                               \
        int index = Table->hash(Key);                                  \
        Changed = 0;                                                   \
        LIST_FOREACH(Table->bucket[index])                             \
        {                                                              \
            if (!Table->cmp(Table->bucket[index]->it->data.key, Key))  \
            {                                                          \
                Ret = Table->bucket[index]->it->data.value;            \
                Changed = 1;                                           \
                break;                                                 \
            }                                                          \
        }                                                              \
    } while(0)

# define HASHTBL_DEL(Table, Key)                                       \
    do {                                                               \
        int index = Table->hash(Key);                                  \
        LIST_FOREACH(Table->bucket[index])                             \
        {                                                              \
            if (!Table->cmp(Table->bucket[index]->it->data.key, Key))  \
            {                                                          \
                LIST_REMOVE(Table->bucket[index],                      \
                           Table->bucket[index]->it);                  \
                break;                                                 \
            }                                                          \
        }                                                              \
    } while(0)

# define HASHTBL_FREE(Table)                                           \
    do {                                                               \
        size_t i;                                                      \
        for (i = 0; i < Table->size; ++i)                              \
            LIST_FREE(Table->bucket[i]);                               \
        free(Table->bucket);                                           \
        free(Table);                                                   \
    } while(0)

# define HASHTBL_DEBUG(Table)                                          \
    do {                                                               \
        size_t i;                                                      \
        for (i = 0; i < Table->size; ++i)\
        {                             \
            fprintf(stderr, "\n%ld : ", i);                            \
            LIST_FOREACH(Table->bucket[i])                             \
                fprintf(stderr, "%d -> ",                              \
                       Table->bucket[i]->it->data.value);              \
        }                                                              \
        fprintf(stderr, "\n");                                         \
    } while(0)

#endif /* !HASHTBL_H */
