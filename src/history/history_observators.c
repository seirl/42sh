#include "shell_private.h"
#include "history.h"
#include "hist_list.h"

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
