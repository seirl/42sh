#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "smalloc.h"
#include "autocompletion.h"
#include "terminal.h"
#include "env.h"
#include "match.h"
#include "readline.h"
#include "wrapper.h"
#include "escape_keys.h"
#include "autocompletion_tools.h"

static s_autocomplete_binaries g_bin =
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

static s_string *first_match(s_term *term)
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
            return string_create_from(g_bin.bins[mid][0]);
    }

    string_free(pat);
    return string_create_from(g_bin.bins[a < 0 ? b : a][0]);
}

static int bins_compare(const void *one, const void *two)
{
    const char * const * const *c_one = one;
    const char * const * const *c_two = two;
    return strcmp(*c_one[0], *c_two[0]);
}

static void replace_word(s_term *term, s_string *word)
{
    size_t start = start_word(term);
    size_t end = end_word(term);
    s_string *new_input = string_create(term->input->len);

    for (size_t i = 0; i < start; i++)
        string_putc(new_input, term->input->buf[i]);
    for (size_t i = 0; i < word->len; i++)
        string_putc(new_input, word->buf[i]);
    for (size_t i = end; i < term->input->len; i++)
        string_putc(new_input, term->input->buf[i]);

    sfree(term->input->buf);
    term->input->buf = new_input->buf;
    term->input->len = new_input->len;
    term->input->max_len = new_input->max_len;
    term->input->read_pos = new_input->read_pos;
    sfree(new_input);

    readline_update_line(term);
    while (term->input_index > start + word->len)
    {
        my_tputs(tgetstr("le", NULL));
        term->input_index--;
    }
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

int autocomplete(s_shell *shell, s_term *term)
{
    size_t i = term->input_index;
    if (!is_blank(term->input->buf[i]))
        handle_escape_key(shell, term, ESCAPE_F);
    i = term->input_index;
    if (i && is_blank(term->input->buf[i - 1]))
        return 0;

    s_string *s = first_match(term);
    replace_word(term, s);
    string_free(s);

    return 0;
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
