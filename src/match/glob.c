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

static s_globr *glob_match(s_globr *g, const char *dir, const char *basename,
                           e_match_flags f)
{
    const char *rdir = dir ? dir : ".";
    DIR *dp = opendir(rdir);
    size_t l = dir ? strlen(rdir) + 1 : 0;
    if (dp != NULL)
    {
        struct dirent *ep;
        while ((ep = readdir(dp)))
        {
            if (!my_fnmatch(basename, ep->d_name))
                if (basename[0] == '.' || ep->d_name[0] != '.' || f & DOTGLOB)
                {
                    char *p = calloc(l + strlen(ep->d_name), sizeof (char));
                    if (dir)
                    {
                        strcpy(p, dir);
                        strcat(p, "/");
                    }
                    strcat(p, ep->d_name);
                    globr_add(g, p);
                    free(p);
                }
        }
        closedir(dp);
    }
    return g;
}

s_globr *my_glob(const char *pattern, e_match_flags flags)
{
    s_globr *g = globr_init();
    if (!has_magic(pattern))
        return glob_literal(g, pattern);
    const char *split = my_strrchr(pattern, '/');
    if (!split)
        return glob_match(g, NULL, pattern, flags);
    char *basename = strdup(split + 1);
    char *dirname = strndup(pattern, split - pattern);
    s_globr *dirs;
    if (strcmp(dirname, basename) && has_magic(dirname))
        dirs = my_glob(dirname, flags);
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

void my_globfree(s_globr *g)
{
    free(g->paths);
    free(g);
}
