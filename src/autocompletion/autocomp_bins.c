#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include "autocomp_bins.h"
#include "autocomp_tools.h"
#include "match.h"
#include "env.h"

static s_autocomp_binaries g_bin =
{
    NULL,
    0,
    0
};

static void try_enlarge_bins(void)
{
    if (g_bin.max_size == 0)
    {
        g_bin.bins = calloc(sizeof (char **), 500);
        g_bin.max_size = 500;
    }

    if (g_bin.size >= g_bin.max_size)
    {
        g_bin.max_size *= 2;
        g_bin.bins = realloc(g_bin.bins, sizeof (char **) * g_bin.max_size);
    }
}

static void add_paths(s_globr *glob_paths)
{
    char *base;
    for (size_t i = 0; i < glob_paths->count; i++)
    {
        g_bin.bins[g_bin.size] = calloc(sizeof (char *), 2);
        base = basename(glob_paths->paths[i]);
        g_bin.bins[g_bin.size][0] = strdup(base);
        g_bin.bins[g_bin.size][1] = glob_paths->paths[i];
        g_bin.size++;
        try_enlarge_bins();
    }
    my_globfree(glob_paths);
}

static int bins_compare(const void *one, const void *two)
{
    const char *const *const *c_one = one;
    const char *const *const *c_two = two;
    return strcmp(*c_one[0], *c_two[0]);
}

s_string *autocomp_bins(s_term *term)
{
    size_t start = start_word(term);
    size_t end = end_word(term);
    s_string *pat = string_extract(term->input, start, end);

    if (!g_bin.bins)
        return pat;

    int a = 0;
    int b = g_bin.size - 1;
    while (a < b)
    {
        int mid = (a + b) / 2;
        int cmp = strncmp(pat->buf, g_bin.bins[mid][0], pat->len);
        if (cmp < 0)
            b = mid - 1;
        else if (cmp > 0)
            a = mid + 1;
        else
        {
            a = mid;
            break;
        }
    }

    size_t i_ret = a < 0 ? b : a;
    if (i_ret < g_bin.size - 1 && !strcmp(g_bin.bins[i_ret][0], pat->buf))
        i_ret++;
    string_free(pat);
    return string_create_from(g_bin.bins[i_ret][0]);
}

void rehash_free(void)
{
    if (g_bin.bins)
    {
        for (size_t i = 0; g_bin.bins[i]; i++)
        {
            free(g_bin.bins[i][0]);
            g_bin.bins[i][0] = NULL;
            g_bin.bins[i][1] = NULL;
            free(g_bin.bins[i]);
            g_bin.bins[i] = NULL;
        }
        g_bin.size = 0;
        free(g_bin.bins);
        g_bin.max_size = 0;
        g_bin.bins = NULL;
    }
}

void rehash(s_shell *shell)
{
    rehash_free();
    try_enlarge_bins();
    char *path = strdup(env_get(shell, "PATH"));
    char *folder = strtok(path, ":");
    while (folder)
    {
        s_string *s_folder = string_create_from(folder);
        string_puts(s_folder, "/*");
        add_paths(my_glob(s_folder->buf, 0));
        folder = strtok(NULL, ":");
        string_free(s_folder);
    }
    g_bin.bins[g_bin.size] = NULL;
    free(path);

    qsort(g_bin.bins, g_bin.size, sizeof (char **), &bins_compare);
}
