#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static char *env_random(void)
{
    int r = rand() % 32768;
    char ret[6];
    snprintf(ret, 6, "%d", r);
    return strdup(ret);
}

char *env_special(char *name)
{
    if (!strcmp(name, "RANDOM"))
        return env_random();
    return NULL;
}
