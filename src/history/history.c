#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "env.h"
#include "history.h"
#include "hist_list.h"

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
    g_hist.lines = h_list_init();
    char *line = NULL;
    while (getline(&line, 0, hist_file))
        h_list_append(g_hist.lines, string_create_from(line));
    g_hist.last_file_entry = g_hist.lines->hd;
}

void close_history(void)
{
    // TODO: Write history
    if (g_hist.lines)
        h_list_delete(g_hist.lines);
        g_hist.lines = NULL;
        g_hist.last_file_entry = NULL;
}

s_hist_entry *query_history(int n)
{
    return h_list_nth(g_hist.lines, n);
}
