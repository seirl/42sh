#include "exec.h"

int redir_list_len(s_ast_redirection_list *redir)
{
    int count = 0;
    while (redir)
    {
        count += 1;
        redir = redir->next;
    }
    return count;
}

s_redir_context *save_redir_context(s_ast_redirection_list *redir)
{
    int size = redir_list_len(redir);
    s_redir_context *context = smalloc(sizeof (s_redir_context) * (size + 1));

    for (int i = 0; i < size; ++i)
    {
        context[i].old_fd = redir->io->io_number;
        context[i].tmp_fd = dup(redir->io->io_number);
        redir = redir->next;
    }
    context[size].old_fd = -1;
    return context;
}

int word_to_fd(s_string *str)
{
    long fd = 0;
    char *endptr = NULL;

    if (str->len == 0)
        return -1;
    if ((string_index(str, 0) == '-') && (string_index(str, 1) == '\0'))
        return -2;
    fd = strtol(str->buf, &endptr, 10);
    if (endptr == str->buf || endptr != 0)
        return -1;

    return fd;
}