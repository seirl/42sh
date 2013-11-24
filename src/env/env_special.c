#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

static char *env_random(void)
{
    int r = rand() % 32768;
    char ret[6];
    snprintf(ret, 6, "%d", r);
    return strdup(ret);
}

static char *env_uid(void)
{
    char buf[32];
    snprintf(buf, 32, "%d", getuid());
    return strdup(buf);
}

char *env_special(char *name)
{
    if (!strcmp(name, "RANDOM"))
        return env_random();
    if (!strcmp(name, "UID"))
        return env_uid();
    return NULL;
}