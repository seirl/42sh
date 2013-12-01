#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "smalloc.h"
#include "builtins.h"
#include "macros.h"
#include "string.h"
#include "log.h"
#include "shell_private.h"
#include "shell.h"
#include "env.h"

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

extern char **environ;

static void id_error(s_shell *shell, char *id)
{
    fprintf(stderr, "42sh: export: `%s': not a valid identifier\n", id);
    shell_status_set(shell, 1);
    sfree(id);
}

static void export_no_params(void)
{
    int i = 0;
    char *s = *environ;

    for (; s; ++i)
    {
        fprintf(stdout, "export %s\n", s);
        s = *(environ + i);
    }
}

static int id_valid(char *buf, int len)
{
    for (int i = 0; i < len; ++i)
    {
        if (!i && !isalpha(buf[0]))
            return 0;
        if (i && !isalnum(buf[i]))
            return 0;
    }
    return 1;
}

static int id_valid_str(s_string *str)
{
    int len = str->len;
    char *buf = string_release(str);
    return id_valid(buf, len);
}

static void export_id_noval(s_shell *shell, s_string *str)
{
    char *id = string_release(str);
    env_set(shell, "", id);
    setenv(id, "", 1);
}

static void export_id(s_shell *shell, s_string *str, int index)
{
    s_string *id_str = string_extract(str, 0, index);
    s_string *val_str = string_extract(str, index + 1, str->len);
    int id_len = id_str->len;
    char *id = string_release(id_str);
    char *val = string_release(val_str);

    if (id_valid(id, id_len))
    {
        env_set(shell, val, id);
        setenv(id, val, 1);
        sfree(val);
        sfree(id);
    }
    else
    {
        id_error(shell, id);
        sfree(val);
    }
    string_free(str);
}

static void export_params(s_shell *shell, int argc, char *argv[])
{
    int i = 1;
    s_string *str = NULL;
    if (!strcmp(argv[1], "-p"))
        ++i;
    for (; i < argc; ++i)
    {
        if (argv[i])
        {
            str = string_create_from(argv[i]);
            int index = 0;
            if ((index = string_search(str, "=")) <= 0)
            {
                if (id_valid_str(str))
                    export_id_noval(shell, str);
                else
                    id_error(shell, argv[i]);
            }
            else
                export_id(shell, str, index);
        }
    }
}

int builtin_export(s_shell *shell, int argc, char *argv[])
{
    shell_status_set(shell, 0);

    if (argc == 1)
        export_no_params();
    else
        export_params(shell, argc, argv);
    return shell->status;
}
