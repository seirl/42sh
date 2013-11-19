#include "exec.h"

void restore_redir_context(s_redir_context *context)
{
    for (int i = 0; context[i].old_fd != -1; ++i)
        dup2(context[i].tmp_fd, context[i].old_fd);
    sfree(context);
}

s_redir_context *exec_redirection(s_ast_redirection_list *redir)
{
    s_redir_context *context = save_redir_context(redir);
    g_shell.status = set_redir(redir);
    return context;
}

int compound_word_len(s_ast_compound_word *word)
{
    int count = 0;

    while (word)
    {
        count += 1;
        word = word->next;
    }
    return count;
}

char **compword_to_argv(s_ast_compound_word *word, int len)
{
    char **cmd_argv = smalloc(sizeof (char *) * (len + 1));
    s_string *str = NULL;

    for (int i = 0; i < len; ++i)
    {
        str = expand_word(word->word);
        cmd_argv[i] = smalloc(sizeof (char) * str->len);
        memcpy(cmd_argv[i], str->buf, str->len);
        word = word->next;
    }
    cmd_argv[len] = NULL;
    return cmd_argv;
}
