#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "autocompletion.h"
#include "terminal.h"
#include "env.h"
#include "match.h"

static s_autocomplete_binaries g_bin =
{
    NULL,
    0,
    0
};

static size_t start_word(s_term *term)
{
    size_t i = term->input_index;
    while (i && term->input->buf[i] != ' ' && term->input->buf[i] != '\t')
        i--;
    return i + 1;
}

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

static void free_binaries(void)
{
    if (g_bin.bins)
    {
        for (int i = 0; g_bin.bins[i]; i++)
        {
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

static void add_paths(s_globr *glob_paths)
{
    for (size_t i = 0; i < glob_paths->count; i++)
    {
        g_bin.bins[g_bin.size] = calloc(sizeof (char *), 2);
        g_bin.bins[g_bin.size][0] = basename(glob_paths->paths[i]);
        g_bin.bins[g_bin.size][1] = glob_paths->paths[i];
        g_bin.size++;
        try_enlarge_bins();
    }
}

static int bins_compare(const void *one, const void *two)
{
    const char * const * const *c_one = one;
    const char * const * const *c_two = two;
    return strcmp(*c_one[0], *c_two[0]);
}

int autocomplete(s_shell *shell, s_term *term)
{
    size_t start = start_word(term);
    (void)start;
    (void)shell;
    // TODO: remove (debug);
    // rehash(shell);
    return 0;
}

void rehash(s_shell *shell)
{
    free_binaries();
    try_enlarge_bins();
    char *folder = strtok(env_get(shell, "PATH"), ":");
    while (folder)
    {
        s_string *s_folder = string_create_from(folder);
        string_puts(s_folder, "/*");
        add_paths(my_glob(s_folder->buf, 0));
        folder = strtok(NULL, ":");
    }
    g_bin.bins[g_bin.size] = NULL;

    qsort(g_bin.bins, g_bin.size, sizeof (char **), &bins_compare);
    // TODO: remove (debug)
    //for (size_t i = 0; g_bin.bins[i]; i++)
    //    printf("%s -> %s\n", g_bin.bins[i][0], g_bin.bins[i][1]);
}
