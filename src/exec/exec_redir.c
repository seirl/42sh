#include "exec.h"
#include "ast.h"
#include "shell_private.h"
#include "smalloc.h"

void restore_redir_context(s_redir_context *context)
{
    if (!context)
        return;
    if (context->old_fd != -1)
    {
        dup2(10, context->old_fd);
        if ((context->redir_type != REDIR_READ_WRITE)
            && (context->redir_type != REDIR_HEREDOC)
            && (context->redir_type != REDIR_HEREDOC_STRIP))
            fcntl(10, F_GETFD);
    }
    sfree(context);
}

s_redir_context *exec_redirection(s_shell *shell,
                                  s_ast_redirection_list *redir)
{
    s_redir_context *context = save_redir_context(redir);
    shell_status_set(shell, set_redir(shell, redir));
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
