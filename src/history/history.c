#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "shell_private.h"
#include "smalloc.h"
#include "env.h"
#include "history.h"
#include "hist_list.h"

static void env_setup(s_shell *shell)
{
    // Query and setup environment.
    if (!env_get(shell, "HISTFILE"))
    {
        s_string *file_path = string_create_from(getenv("HOME"));
        s_string *filename = string_create_from("/.42sh_history");
        string_cat(file_path, filename);
        env_set(shell, file_path->buf, "HISTFILE");
        string_free(file_path);
        string_free(filename);
    }
    if (!env_get(shell, "HISTFILESIZE"))
        env_set(shell, "500", "HISTFILESIZE");
}

static void read_entries(s_shell *shell, FILE *hist_file)
{
    char *line = NULL;
    size_t buf_size = 0;
    int line_len = 0;
    time_t date = time(NULL);

    while ((line_len = getline(&line, &buf_size, hist_file)) != -1)
    {
        if (*line && line[line_len - 1] == '\n')
            line[line_len - 1] = '\0';

        if (*line == '#')
        {
            sscanf(line, "#%ld", &date);
            free(line);
            line = NULL;
            if ((line_len = getline(&line, &buf_size, hist_file)) == -1)
                break;
            if (*line && line[line_len - 1] == '\n')
                line[line_len - 1] = '\0';
        }

        h_list_append(shell->history->lines, string_create_from(line), date);
        free(line);
        line = NULL;
    }

    if (line)
        free(line);
}

static void history_open(s_shell *shell)
{
    env_setup(shell);
    FILE *hist_file;

    if (access(env_get(shell, "HISTFILE"), F_OK) == -1)
        if ((hist_file = fopen(env_get(shell, "HISTFILE"), "w")) != NULL)
            fclose(hist_file);

    if (access(env_get(shell, "HISTFILE"), R_OK) == -1)
        return;

    hist_file = fopen(env_get(shell, "HISTFILE"), "r");
    shell->history = smalloc(sizeof (s_history));
    shell->history->lines = h_list_init();

    read_entries(shell, hist_file);

    shell->history->last_file_entry = shell->history->lines->hd;
    fclose(hist_file);
}

static void history_write_rec(s_shell *shell, FILE *f, s_hist_entry *e)
{
    if (!e || e == shell->history->last_file_entry)
        return;
    history_write_rec(shell, f, e->next);

    fprintf(f, "#%ld\n", e->date);
    fprintf(f, "%s\n", e->line->buf);
}

static void history_write(s_shell *shell)
{
    if (!shell->history)
        history_open(shell);
    if (!shell->history)
        return;
    FILE *f = fopen(env_get(shell, "HISTFILE"), "a+");
    if (f)
    {
        history_write_rec(shell, f, shell->history->lines->hd);
        fclose(f);
    }
}

void history_close(s_shell *shell)
{
    if (!shell->history)
        return;
    history_write(shell);

    h_list_delete(shell->history->lines);
    shell->history->lines = NULL;
    shell->history->last_file_entry = NULL;
    sfree(shell->history);
    shell->history = NULL;
}

void history_reset(s_shell *shell)
{
    if (!shell->history)
        history_open(shell);
    if (!shell->history)
        return;
    h_list_delete(shell->history->lines);
    shell->history->lines = h_list_init();
}

s_hist_entry *history_get(s_shell *shell, int n)
{
    if (!shell->history)
        history_open(shell);
    if (!shell->history)
        return NULL;
    return h_list_nth(shell->history->lines, n);
}

int history_size(s_shell *shell)
{
    if (!shell->history)
        history_open(shell);
    if (shell->history && shell->history->lines)
        return shell->history->lines->size;
    return 0;
}

void history_add(s_shell *shell, s_string *line)
{
    if (!shell->history)
        history_open(shell);
    if (!shell->history)
        return;
    h_list_append(shell->history->lines, line, time(NULL));
}
