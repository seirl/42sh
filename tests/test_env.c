#include <stdio.h>
#include <stdlib.h>
#include "env.h"
#include "smalloc.h"

int test_0()
{
    return env_get("test") ? 1 : 0;
}

int test_1()
{
    env_set("foo", "bar");
    printf("%s", env_get("bar"));
    return 0;
}

int test_2()
{
    env_set("foo", "bar");
    env_unset("bar");
    printf("%s", env_get("bar"));
    return 0;
}

int test_3()
{
    env_set("bar", "foo");
    env_set("baz", "foo");
    printf("%s", env_get("foo"));
    return 0;
}

int test_4()
{
    env_set("foo_bar", "foo");
    env_set("tata", "toto");
    env_set("bar_bar", "bar");
    env_set("baz_bar", "baz");
    env_unset("toto");
    env_set("test_bar", "test");
    printf("%s %s %s %s %s", env_get("baz"), env_get("test"),
                             env_get("bar"), env_get("foo"),
                             env_get("toto"));
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    int f = atoi(argv[1]);
    int ret = 0;
    if (f == 0)
        ret = test_0();
    if (f == 1)
        ret = test_1();
    if (f == 2)
        ret = test_2();
    if (f == 3)
        ret = test_3();
    if (f == 4)
        ret = test_4();
    env_free();
    smalloc_clean();
    return ret;
}
