#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "shell.h"
#include "env.h"
#include "string_utils.h"

static char *env_random(void)
{
    int r = rand() % 32768;
    static char ret[6];
    snprintf(ret, 6, "%d", r);
    return ret;
}

static char *env_uid(void)
{
    static char buf[32];
    snprintf(buf, 32, "%d", getuid());
    return buf;
}

static char *env_pid(void)
{
    static char buf[32];
    snprintf(buf, 32, "%d", getpid());
    return buf;
}

static void env_sharp(s_shell *shell)
{
    int i = 1;
    char buf[6];
    do {
        snprintf(buf, 6, "%d", i);
        ++i;
    } while (env_get(shell, buf));
    env_set(shell, buf, "#");
}

static void env_star(s_shell *shell)
{
    int i = 1;
    char buf[6];
    s_string *ret = string_create(0);
    while (1)
    {
        snprintf(buf, 6, "%d", i);
        char *var = env_get(shell, buf);
        if (var == NULL)
            break;
        if (i != 1)
            string_putc(ret, ' ');
        string_puts(ret, var);
        ++i;
    }
    env_set(shell, ret->buf, "*");
    string_free(ret);
}

static void env_at(s_shell *shell)
{
    int i = 1;
    char buf[6];
    s_string *ret = string_create(0);
    while (1)
    {
        snprintf(buf, 6, "%d", i);
        char *var = env_get(shell, buf);
        if (var == NULL)
            break;
        if (i != 1)
            string_putc(ret, ' ');
        string_puts(ret, var);
        ++i;
    }
    env_set(shell, ret->buf, "@");
    string_free(ret);
}

char *env_special(s_shell *shell, char *name)
{
    if (!strcmp(name, "RANDOM"))
        return env_random();
    else if (!strcmp(name, "UID"))
        return env_uid();
    else if (!strcmp(name, "$"))
        return env_pid();
    else if (!strcmp(name, "#"))
        env_sharp(shell);
    else if (!strcmp(name, "*"))
        env_star(shell);
    else if (!strcmp(name, "@"))
        env_at(shell);
    return NULL;
}
