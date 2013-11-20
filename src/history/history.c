#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "env.h"
#include "history.h"
#include "hist_list.h"

static s_history *g_hist;

static void env_setup(void)
{
    // Query and setup environment.
    if (!env_get("HISTFILE"))
    {
        s_string *file_path = string_create_from(getenv("HOME"));
        s_string *filename = string_create_from("/.42sh_history");
        string_cat(file_path, filename);
        env_set(file_path->buf, "HISTFILE");
        string_free(file_path);
        string_free(filename);
    }
    if (!env_get("HISTFILESIZE"))
        env_set("500", "HISTFILESIZE");
}

static void history_open(void)
{
    env_setup();
    int error;
    FILE *hist_file;

    if ((error = access(env_get("HISTFILE"), F_OK)) == -1)
        if ((hist_file = fopen(env_get("HISTFILE"), "w")) != NULL)
            fclose(hist_file);

    if ((error = access(env_get("HISTFILE"), R_OK)) == -1)
        return;

    hist_file = fopen(env_get("HISTFILE"), "r");
    g_hist = malloc(sizeof (s_history));
    g_hist->lines = h_list_init();
    char *line = NULL;
    size_t buf_size = 0;
    int line_len = 0;

    while ((line_len = getline(&line, &buf_size, hist_file)) != -1)
    {
        if (*line && line[line_len - 1] == '\n')
            line[line_len - 1] = '\0';
        h_list_append(g_hist->lines, string_create_from(line));
        free(line);
        line = NULL;
    }
    free(line);

    g_hist->last_file_entry = g_hist->lines->hd;
    fclose(hist_file);
}

static void history_write_rec(FILE *f, s_hist_entry *e)
{
    if (!e)
        return;
    history_write_rec(f, e->next);
    fprintf(f, "%s\n", e->line->buf);
}

static void history_write(void)
{
    if (!g_hist)
        history_open();
    if (!g_hist)
        return;
    FILE *f = fopen(env_get("HISTFILE"), "w+");
    if (f)
    {
        history_write_rec(f, g_hist->lines->hd);
        fclose(f);
    }
}

void history_close(void)
{
    history_write();
    if (!g_hist)
        return;

    h_list_delete(g_hist->lines);
    g_hist->lines = NULL;
    g_hist->last_file_entry = NULL;
    free(g_hist);
    g_hist = NULL;
}

s_hist_entry *history_get(int n)
{
    if (!g_hist)
        history_open();
    if (!g_hist)
        return NULL;
    return h_list_nth(g_hist->lines, n);
}

int history_size(void)
{
    if (!g_hist)
        history_open();
    if (g_hist && g_hist->lines)
        return g_hist->lines->size;
    return 0;
}

void history_add(s_string *line)
{
    if (!g_hist)
        history_open();
    if (!g_hist)
        return;
    h_list_append(g_hist->lines, line);
}
