#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shopt.h"

static s_shopt *shopt_get_ptr(size_t *size)
{
    static s_shopt shell_options[] =
    {
        { "ast_print", 0 },
        { "dotglob", 0 },
        { "expand_aliases", 1 },
        { "extglob", 0 },
        { "nocaseglob", 0 },
        { "nullglob", 0 },
        { "sourcepath", 1 },
        { "xpg_echo", 0 },
    };
    *size = sizeof (shell_options) / sizeof (s_shopt);
    return shell_options;
}

int shopt_set(const char *shopt, int value)
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
    {
        if (!strcmp(shopt, shell_options[i].name))
        {
            shell_options[i].value = value;
            return 1;
        }
    }
    return 0;
}

int shopt_get(const char *shopt)
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
        if (!strcmp(shopt, shell_options[i].name))
            return shell_options[i].value;
    return -1;
}

void shopt_print()
{
    size_t size;
    s_shopt *shell_options = shopt_get_ptr(&size);
    for (size_t i = 0 ; i < size; ++i)
    {
        fprintf(stdout, "%s\t%s\n", shell_options[i].name,
               shell_options[i].value == 1 ? "on" : "off");
    }
}
