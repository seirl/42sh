#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "terminal.h"
#include "match.h"
#include "autocomp_tools.h"

static int is_in(char *s, char c)
{
    for (size_t i = 0; i < strlen(s); i++)
        if (s[i] == c)
            return 1;
    return 0;
}

static int is_dir(s_string *path)
{
    if (!path)
        return 0;
    struct stat s;
    if (stat(path->buf, &s) == 0)
        if (S_ISDIR(s.st_mode))
            return 1;
    return 0;
}

s_string *autocomp_paths(s_term *term)
{
    size_t start = start_word(term);
    size_t end = end_word(term);
    s_string *pat = string_extract(term->input, start, end);
    string_puts(pat, "*");
    s_globr *glob_paths = my_glob(pat->buf, 0);
    string_del_from_end(pat, 2);

    s_string *ret = NULL;
    for (size_t i = 0; i < glob_paths->count; i++)
        if (!strncmp(glob_paths->paths[i], pat->buf, pat->len))
            if (!is_in(glob_paths->paths[i], '*'))
                ret = string_create_from(glob_paths->paths[i]);

    if (is_dir(ret))
        string_puts(ret, "/");
    my_globfree(glob_paths);
    string_free(pat);
    return ret;
}
