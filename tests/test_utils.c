#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"
#include "string_utils.h"
#include "smalloc.h"

HASHTBL(char *, char *, ctable, null_free, null_free);
HASHTBL(s_string *, s_string *, stable, string_free, string_free);

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

static int test_3()
{
    s_ctable *t;
    HASHTBL_INIT(t, 10, my_hash, my_cmp, NULL);
    HASHTBL_SET(t, "0", "zero");
    if (!value_is(t, "zero", "0"))
        return 1;
    HASHTBL_SET(t, "1", "zero");
    if (!value_is(t, "zero", "1"))
        return 1;
    HASHTBL_FREE(t);
    return 0;
}

static int test_4()
{
    s_ctable *t;
    HASHTBL_INIT(t, 10, my_hash, my_cmp, NULL);
    HASHTBL_DEL(t, "foo");
    HASHTBL_FREE(t);
    return 0;
}

static int test_5()
{
    s_stable *t;
    HASHTBL_INIT(t, 10, hash_string, cmp_string, free_stable);
    s_string *s1 = string_create_from("foo");
    s_string *s2 = string_create_from("bar");
    HASHTBL_SET(t, s1, s2);
    HASHTBL_FREE(t);
    return 0;
}

static int test_6()
{
    s_stable *t;
    HASHTBL_INIT(t, 10, hash_string, cmp_string, free_stable);
    s_string *s1 = string_create_from("foo");
    s_string *s2 = string_create_from("bar");
    s_string *s3 = string_create_from("egg");
    s_string *s4 = string_create_from("bar");
    HASHTBL_SET(t, s1, s2);
    HASHTBL_SET(t, s3, s4);
    HASHTBL_FREE(t);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    int ret = 0;
    int n = atoi(argv[1]);
    if (n == 1)
        ret = test_1();
    else if (n == 2)
        ret = test_2();
    else if (n == 3)
        ret = test_3();
    else if (n == 4)
        ret = test_4();
    else if (n == 5)
        ret = test_5();
    else if (n == 6)
        ret = test_6();
    smalloc_clean();
    return ret;
}
