#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"
#include "string_utils.h"
#include "smalloc.h"

int test_1()
{
    s_hashtbl *h = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    hashtbl_free(h);
    return 0;
}

int test_2()
{
    s_hashtbl *h = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    hashtbl_set(h, strdup("foo"), strdup("bar"));
    if (hashtbl_get(h, "bar") == NULL)
        return 1;
    hashtbl_unset(h, "bar");
    if (hashtbl_get(h, "bar"))
        return 1;
    hashtbl_free(h);
    return 0;
}

int test_3()
{
    s_hashtbl *h = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    hashtbl_set(h, strdup("foo"), strdup("bar"));
    hashtbl_set(h, strdup("baz"), strdup("bar"));
    if (hashtbl_get(h, "bar") == NULL)
        return 1;
    hashtbl_free(h);
    return 0;
}

int test_4()
{
    s_hashtbl *h = hashtbl_init(hash_char, cmp_char, free_char, free_char);
    hashtbl_unset(h, "bar");
    hashtbl_free(h);
    return 0;
}

unsigned long dummy_hash(void *s)
{
    char *str = s;
    return str[0];
}

int test_5()
{
    s_hashtbl *h = hashtbl_init(dummy_hash, cmp_char, NULL, NULL);
    hashtbl_set(h, "a", "bar");
    hashtbl_set(h, "b", "baz");
    hashtbl_set(h, "z", "baf");
    hashtbl_set(h, "d", "xor");
    hashtbl_set(h, "c", "baf");
    s_hash_elt *it = NULL;
    char *res;
    while ((res = hashtbl_iter(h, &it)))
        printf("%s\n", res);
    hashtbl_free(h);
    return 0;
}


int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    int f = atoi(argv[1]);
    if (f == 1)
        return test_1();
    if (f == 2)
        return test_2();
    if (f == 3)
        return test_3();
    if (f == 4)
        return test_4();
    if (f == 5)
        return test_5();
    return 0;
}
