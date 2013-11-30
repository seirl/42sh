#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "match.h"

/*
** On some Linux distributions, strchr() segfaults...
*/
static const char *my_strrchr(const char *haystack, char needle)
{
    const char *r = NULL;
    while (*haystack)
    {
        if (*haystack == needle)
            r = haystack;
        haystack++;
    }
    return r;
}

static int has_magic(const char *pattern)
{
    const char *magic = "*?[{";
    for (unsigned i = 0; pattern[i]; i++)
        for (unsigned j = 0; magic[j]; j++)
            if (pattern[i] == magic[j])
                return 1;
    return 0;
}

static s_globr *globr_init(void)
{
    s_globr *g = malloc(sizeof (s_globr));
    g->count = 0;
    g->size_max = 8;
    g->paths = malloc(sizeof (char *) * g->size_max);
    return g;
}

static void globr_add(s_globr *g, const char *path)
{
    if (g->count >= g->size_max)
    {
        g->size_max *= 2;
        g->paths = realloc(g->paths, sizeof (char *) * g->size_max);
    }
    g->paths[g->count++] = strdup(path);
}

static s_globr *glob_literal(s_globr *g, const char *filename)
{
    if (access(filename, F_OK))
        globr_add(g, filename);
    return g;
}

static s_globr *glob_match(s_globr *g, const char *dir, const char *base,
                           e_match_flags f)
{
    const char *rdir = dir ? dir : ".";
    DIR *dp = opendir(rdir);
    size_t l = 1 + (dir ? strlen(rdir) + 1 : 0);
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)))
            if (!optmatch(base, ep->d_name, f))
                if (base[0] == '.' || ep->d_name[0] != '.' || f & MATCH_DOT)
                {
                    char *p = calloc(l + strlen(ep->d_name), sizeof (char));
                    if (dir)
                    {
                        strcpy(p, dir);
                        if (dir[strlen(dir) - 1] != '/')
                            strcat(p, "/");
                    }
                    strcat(p, ep->d_name);
                    globr_add(g, p);
                    free(p);
                }
        closedir(dp);
    }
    return g;
}

static s_globr *glob_rec(const char *pattern, e_match_flags flags);

static s_globr *glob_loop(const char *pattern, e_match_flags flags,
                          s_globr *g, const char *split)
{
    unsigned add = (split == pattern) ? 1 : 0;
    char *dirname = strndup(pattern, split - pattern + add);
    char *basename = strdup(split + 1);
    s_globr *dirs;
    if (has_magic(dirname))
        dirs = glob_rec(dirname, flags);
    else
    {
        dirs = globr_init();
        globr_add(dirs, dirname);
    }
    for (unsigned i = 0; i < dirs->count; i++)
        if (has_magic(basename))
            glob_match(g, dirs->paths[i], basename, flags);
        else
            glob_literal(g, pattern);
    free(basename);
    free(dirname);
    my_globfree(dirs);
    return g;
}

static s_globr *glob_rec(const char *pattern, e_match_flags flags)
{
    s_globr *g = globr_init();
    if (!has_magic(pattern))
        return glob_literal(g, pattern);
    const char *split = my_strrchr(pattern, '/');
    if (!split)
        return glob_match(g, NULL, pattern, flags);
    glob_loop(pattern, flags, g, split);
    return g;
}

s_globr *my_glob(const char *pattern, e_match_flags flags)
{
    s_globr *g = glob_rec(pattern, flags);
    if (!g->count && !(flags & MATCH_NULL))
        globr_add(g, pattern);
    return g;
}

void my_globfree(s_globr *g)
{
    for (unsigned i = 0; i < g->count; i++)
        free(g->paths[i]);
    free(g->paths);
    free(g);
}
