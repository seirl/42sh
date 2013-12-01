#include <stdlib.h>
#include <string.h>

#include "expand.h"
#include "match.h"
#include "shopt.h"

static const struct
{
    e_match_flags f;
    char *opt;
} opt_flags[] =
{
    { MATCH_EXT, "extglob" },
    { MATCH_NOCASE, "nocaseglob" },
    { MATCH_DOT, "dotglob" },
    { MATCH_NULL, "nullglob" },
};

static int cmpstringp(const void *s1, const void *s2)
{
    char *const *str1 = s1;
    char *const *str2 = s2;
    return strcmp(*str1, *str2);
}

s_string *expand_glob(s_shell *shell, s_string *word)
{
    e_match_flags glob_flags = 0;
    if (!has_magic(word->buf))
        return string_duplicate(word);
    for (unsigned i = 0; i < sizeof (opt_flags) / sizeof (*opt_flags); i++)
        if (shopt_get(shell, opt_flags[i].opt))
            glob_flags |= opt_flags[i].f;
    s_globr *g = my_glob(word->buf, glob_flags);
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
