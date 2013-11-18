#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "env.h"
#include "history.h"

static s_history g_hist;

void open_history(void)
{
    if (!env_get("HISTFILE"))
        env_set("HISTFILE", "~/.sh_history");
    if (!env_get("HISTFILESIZE"))
        env_set("HISTFILESIZE", "500");

    if (access(env_get("HISTFILE"), R_OK))
        return;

    FILE *hist_file = fopen(env_get("HISTFILE"), "r");
    // TODO: try to truncate the start of the file if there is more line than
    // HISTFILESIZE
    LIST_INIT(g_hist.lines, string_free);
    char *line = NULL;
    while (getline(&line, 0, hist_file))
        LIST_INSERT_HEAD(g_hist.lines, string_create_from(line));
    g_hist.last_file_entry = g_hist.lines->head;
}

void close_history(void)
{
    // TODO: Write history
    if (g_hist.lines)
        LIST_FREE(g_hist.lines);
}

s_string *query_history(int n)
{
    s_Node_hist_lines *elt = NULL;
    LIST_AT(g_hist.lines, n, elt);
    return elt->data;
}
