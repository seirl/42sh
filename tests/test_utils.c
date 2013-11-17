#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"
#include "string_utils.h"

HASHTBL(char *, char *, ctable);
HASHTBL(s_string *, s_string *, stable);

static int test_1()
{
    s_ctable *t;
    HASHTBL_INIT(t, 10, hash_char, cmp_char, NULL);
    HASHTBL_FREE(t);
    return 0;
}

static unsigned long my_hash(char *s1)
{
    (void)s1;
    return 0;
}

static int my_cmp(char *s1, char *s2)
{
    return strcmp(s1, s2);
}

static int value_is(s_ctable *t, char *key, char *res)
{
    int changed = 0;
    char *ret;
    HASHTBL_GET(t, key, ret, changed);
    if (changed)
    {
        if (!strcmp(res, ret))
            return 1;
    }
    return 0;
}

static int test_2()
{
    s_ctable *t;
    HASHTBL_INIT(t, 10, my_hash, my_cmp, NULL);
    HASHTBL_SET(t, "0", "zero");
    HASHTBL_SET(t, "1", "un");
    HASHTBL_SET(t, "2", "deux");
    if (!value_is(t, "zero", "0"))
        return 1;
    if (!value_is(t, "un", "1"))
        return 1;
    if (!value_is(t, "deux", "2"))
        return 1;
    HASHTBL_DEL(t, "un");
    if (value_is(t, "un", "1"))
        return 1;
    HASHTBL_FREE(t);
    return 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    int n = atoi(argv[1]);
    if (n == 1)
        return test_1();
    if (n == 2)
        return test_2();
    return EXIT_SUCCESS;
}
