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
    hashtbl_set(h, strdup("foo"), strdup("bar"));
    if (hashtbl_get(h, "bar") == NULL)
        return 1;
    hashtbl_unset(h, "bar");
    if (hashtbl_get(h, "bar"))
        return 1;
    hashtbl_free(h);
    return 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (test_1())
        return 1;
    return 0;
}
