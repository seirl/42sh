#include <string.h>
#include <stdio.h>
#include "input_string.h"
#include "input.h"
#include "lexer.h"
#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "smalloc.h"
#include "match.h"

static char **push_arg(char **argv, char *arg, int *len)
{
    *len += 1;
    argv = srealloc(argv, sizeof (char *) * (*len + 1));
    int str_len = strlen(arg) + 1;
    argv[*len - 1] = smalloc(sizeof (char) * str_len);
    memcpy(argv[*len - 1], arg, str_len);
    argv[*len - 1][str_len - 1] = 0;
    return argv;
}

char **expand_argv(char **argv, int *max_len)
{
    int len = 0;
    char **ret = smalloc(sizeof (char *) * 1);
    for (int i = 0; i < *max_len; ++i)
    {
        s_globr *gl = my_glob(argv[i], 0);
        for (unsigned j = 0; j < gl->count; ++j)
            ret = push_arg(ret, gl->paths[j], &len);
        my_globfree(gl);
    }

    ret = srealloc(ret, sizeof (char *) * (len + 1));
    ret[len] = NULL;

    for (int i = 0; i < *max_len; ++i)
        sfree(argv[i]);
    sfree(argv);
    *max_len = len;
    return ret;
}
