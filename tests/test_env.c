#include <stdio.h>
#include <stdlib.h>
#include "env.h"

int test_0()
{
    return env_get("test") ? 1 : 0;
}

int test_1()
{
    env_set("foo", "bar");
    printf("%s", env_get("foo"));
    return 0;
}

int test_2()
{
    env_set("foo", "bar");
    env_unset("foo");
    printf("%s", env_get("foo"));
    return 0;
}

int test_3()
{
    env_set("foo", "bar");
    env_set("foo", "baz");
    printf("%s", env_get("foo"));
    return 0;
}

int test_4()
{
    env_set("foo", "foo_bar");
    env_set("toto", "tata");
    env_set("bar", "bar_bar");
    env_set("baz", "baz_bar");
    env_unset("toto");
    env_set("test", "test_bar");
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
    if (f == 0)
        return test_0();
    if (f == 1)
        return test_1();
    if (f == 2)
        return test_2();
    if (f == 3)
        return test_3();
    if (f == 4)
        return test_4();
    env_free();
    return 0;
}
