#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "string_utils.h"
#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "expand_wordlist.h"
#include "expand.h"
#include "env.h"
#include "ifs.h"
#include "match.h"

s_string *expand_glob(s_shell *shell, s_string *word)
{
    (void)shell;
    s_globr *g = my_glob(word->buf, 0);
    if (g->count == 0)
        return word;
    s_string *ret = string_create(0);
    for (unsigned i = 0; i < g->count; ++i)
    {
        string_puts(ret, g->paths[i]);
        if (i + 1 != g->count)
            string_putc(ret, ' ');
    }
    my_globfree(g);
    return ret;
}
