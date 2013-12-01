#include "exec.h"

#include "smalloc.h"

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

void set_default_io_number(s_ast_redirection_list *redir)
{
    if (redir && !redir->io)
    {
        redir->io = ast_io_number_new();
        if (redir->type == REDIR_WRITE)                 /** >   */
            redir->io->io_number = 1;
        else if (redir->type == REDIR_READ)             /** <   */
            redir->io->io_number = 0;
        else if (redir->type == REDIR_HEREDOC)          /** <<  */
            redir->io->io_number = 0;
        else if (redir->type == REDIR_HEREDOC_STRIP)    /** <<- */
            redir->io->io_number = 0;
        else if (redir->type == REDIR_WRITE_UPDATE)     /** >>  */
            redir->io->io_number = 1;
        else if (redir->type == REDIR_DUPLICATE_INPUT)  /** <&  */
            redir->io->io_number = 0;
        else if (redir->type == REDIR_DUPLICATE_OUTPUT) /** >&  */
            redir->io->io_number = 1;
        else if (redir->type == REDIR_CLOBBER)          /** >|  */
            redir->io->io_number = 1;
        else if (redir->type == REDIR_READ_WRITE)       /** <>  */
            redir->io->io_number = 0;
    }
}

s_redir_context *save_redir_context(s_ast_redirection_list *redir)
{
    int size = redir_list_len(redir);
    s_redir_context *context = smalloc(sizeof (s_redir_context) * (size + 1));
    set_default_io_number(redir);

    for (int i = 0; i < size; ++i)
    {
        context[i].old_fd = redir->io->io_number;
        context[i].tmp_fd = 10;
        context[i].redir_type = redir->type;
        redir = redir->next;
    }
    context[size].old_fd = -1;
    return context;
}

int word_to_fd(s_string *str, int mode)
{
    long fd = 0;
    char *endptr = NULL;

    if (str->len == 0)
        return -1;
    if ((string_index(str, 0) == '-') && (string_index(str, 1) == '\0'))
        return -2;
    fd = strtol(str->buf, &endptr, 10);
    if ((endptr != 0) && strcmp("", endptr))
        fd = open(str->buf, mode);
    return fd;
}
