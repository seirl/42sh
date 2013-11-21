#include <stdio.h>
#include <stdlib.h>

#include "shell.h"
#include "env.h"
#include "smalloc.h"

int test_0(s_shell *shell)
{
    return env_get(shell, "test") ? 1 : 0;
}

int test_1(s_shell *shell)
{
    env_set(shell, "foo", "bar");
    printf("%s", env_get(shell, "bar"));
    return 0;
}

int test_2(s_shell *shell)
{
    env_set(shell, "foo", "bar");
    env_unset(shell, "bar");
    printf("%s", env_get(shell, "bar"));
    return 0;
}

int test_3(s_shell *shell)
{
    env_set(shell, "bar", "foo");
    env_set(shell, "baz", "foo");
    printf("%s", env_get(shell, "foo"));
    return 0;
}

int test_4(s_shell *shell)
{
    env_set(shell, "foo_bar", "foo");
    env_set(shell, "tata", "toto");
    env_set(shell, "bar_bar", "bar");
    env_set(shell, "baz_bar", "baz");
    env_unset(shell, "toto");
    env_set(shell, "test_bar", "test");
    printf("%s %s %s %s %s", env_get(shell, "baz"), env_get(shell, "test"),
                             env_get(shell, "bar"), env_get(shell, "foo"),
                             env_get(shell, "toto"));
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    int f = atoi(argv[1]);
    int ret = 0;
    s_shell *shell = shell_new();
    if (f == 0)
        ret = test_0(shell);
    if (f == 1)
        ret = test_1(shell);
    if (f == 2)
        ret = test_2(shell);
    if (f == 3)
        ret = test_3(shell);
    if (f == 4)
        ret = test_4(shell);
    env_free(shell);
    shell_delete(shell);
    smalloc_clean();
    return ret;
}
