#ifndef HASHTBL_H
# define HASHTBL_H

# define HASHTBL_SIZE 100

struct hash_elt
{
    void *value;
    void *key;
    struct hash_elt *next;
};
typedef struct hash_elt s_hash_elt;

struct hashtbl
{
    unsigned long (*hash)(void *);
    int (*cmp)(void *, void *);
    void (*free_key)(void *);
    void (*free_value)(void *);
    s_hash_elt **bucket;
};
typedef struct hashtbl s_hashtbl;

s_hashtbl *hashtbl_init(unsigned long (*hash)(void *),
                        int (*cmp)(void *, void *),
                        void (*free_key)(void *),
                        void (*free_value)(void *));
void hashtbl_set(s_hashtbl *h, void *value, void *key);
void *hashtbl_get(s_hashtbl *h, void *key);
void hashtbl_unset(s_hashtbl *h, void *key);
void hashtbl_free(s_hashtbl *h);

// hash_char.c
unsigned long hash_char(void *s);
int cmp_char(void *s1, void *s2);
void free_char(void *s);

// hash_string.c
unsigned long hash_string(void *s);
int cmp_string(void *s1, void *s2);
void free_string(void *s);

#endif /* !HASHTBL_H */
