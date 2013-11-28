#include <stdlib.h>
#include "shell.h"
#include "shell_private.h"
#include "getopt.h"
#include "shopt.h"
#include "env.h"
#include "history.h"
#include "smalloc.h"

static const s_param shopt_param[] =
{
    { 'c', NULL, 0, 0, NEW_ARG },
    { 'r', NULL, 0, 1, NEW_ARG },
};

static void usage(void)
{
    fprintf(stdout, "history: usage: history [-c] [-r [filename]] [n]\n");
}

static void history_clear(s_shell *shell)
{
    history_reset(shell);
}

static void history_set_filename(s_shell *shell, char *filename)
{
    history_add_from(shell, filename);
}

static void history_list(s_shell *shell, s_opt *opt)
{
    size_t count = history_size(shell);
    if (opt->trailing_count)
        count = atoi(opt_trailing_arg(opt, 0));

    char *time_format = "";
    if (env_get(shell, "HISTTIMEFORMAT"))
        time_format = env_get(shell, "HISTTIMEFORMAT");

    for (size_t i = count - 1; i; i--)
    {
        s_hist_entry *entry = history_get(shell, i);
        if (!entry)
            break;

        char *date = malloc(sizeof (char) * 1024);
        strftime(date, 1024, time_format, localtime(&entry->date));
        printf("%5zu  %s%s\n", count - i, date, entry->line->buf);
        free(date);
    }
}

int builtin_history(s_shell *shell, int argc, char *argv[])
{
    if (!shopt_get(shell, "history"))
        return 0;

    s_opt *opt = opt_init(shopt_param, 2);
    if (opt_parse(argc, argv, opt))
    {
        usage();
        return 2;
    }
    char *filename = smalloc(sizeof (char) * 1024);
    if (opt_get(opt, "c", NULL))
        history_clear(shell);
    else if (opt_get(opt, "r", &filename))
        history_set_filename(shell, filename);
    else
        history_list(shell, opt);

    opt_free(opt);
    sfree(filename);
    return 0;
}
