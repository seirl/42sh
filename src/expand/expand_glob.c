#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "string_utils.h"
#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "expand.h"
#include "env.h"
#include "ifs.h"
#include "match.h"

static int cmpstringp(const void *s1, const void *s2)
{
    char *const *str1 = s1;
    char *const *str2 = s2;
    return strcmp(*str1, *str2);
}

s_string *expand_glob(s_shell *shell, s_string *word)
{
    (void)shell;
    s_globr *g = my_glob(word->buf, 0);
    s_string *ret = string_create(0);
    qsort(g->paths, g->count, sizeof (char *), cmpstringp);
    for (unsigned i = 0; i < g->count; ++i)
    {
        string_puts(ret, g->paths[i]);
        if (i + 1 != g->count)
            string_putc(ret, ' ');
    }
    my_globfree(g);
    return ret;
}
