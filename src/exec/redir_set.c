#include "exec.h"

static void exec_redir_write(s_ast_redirection_list *redir,
                             int fd)
{
    s_string *filename = expand_compound(redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_TRUNC,
                   666)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    close(fd);
}

static void exec_redir_read(s_ast_redirection_list *redir, int fd)
{
    s_string *filename = expand_compound(redir->word);
    if ((fd = open(filename->buf, O_RDONLY)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    close(fd);
}

static void exec_redir_heredoc(s_ast_redirection_list *redir)
{
    write(redir->io->io_number,
          redir->heredoc->heredoc->buf,
          redir->heredoc->heredoc->len);
}

static void exec_redir_dupout(s_ast_redirection_list *redir,
                              int fd)
{
    s_string *filename = expand_compound(redir->word);
    fd = word_to_fd(filename);
    if (fd == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    else if (fd == -2)
        close(fd); /* FIXME error handling */
    dup2(fd, redir->io->io_number);
}

static void exec_redir_dupin(s_ast_redirection_list *redir,
                             int fd)
{
    s_string *filename = expand_compound(redir->word);
    fd = word_to_fd(filename);
    if (fd == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    else if (fd == -2)
        close(fd); /* FIXME error handling */
    dup2(fd, redir->io->io_number);
}

static void exec_redir_clobber(s_ast_redirection_list *redir,
                               int fd)
{
    s_string *filename = expand_compound(redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_TRUNC,
                   666)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    close(fd);
}

static void exec_redir_readwrite(s_ast_redirection_list *redir,
                                 int fd)
{
    s_string *filename = expand_compound(redir->word);
    if ((fd = open(filename->buf, O_CREAT | O_RDWR | O_TRUNC)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    close(fd);
}

static void exec_redir_writeup(s_ast_redirection_list *redir,
                               int fd)
{
    s_string *filename = expand_compound(redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_APPEND,
                   666)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    close(fd);
}

static void exec_redir_type(s_ast_redirection_list *redir,
                            int fd)
{
    if (redir->type == REDIR_WRITE)                 /** >   */
        exec_redir_write(redir, fd);
    else if (redir->type == REDIR_WRITE_UPDATE)     /** >>  */
        exec_redir_writeup(redir, fd);
    else if (redir->type == REDIR_READ)             /** <   */
        exec_redir_read(redir, fd);
    else if (redir->type == REDIR_HEREDOC)          /** <<  */
        exec_redir_heredoc(redir);
    else if (redir->type == REDIR_HEREDOC_STRIP)    /** <<- */
        exec_redir_heredoc(redir);
    else if (redir->type == REDIR_DUPLICATE_INPUT)  /** <&  */
        exec_redir_dupin(redir, fd);
    else if (redir->type == REDIR_DUPLICATE_OUTPUT) /** >&  */
        exec_redir_dupout(redir, fd);
    else if (redir->type == REDIR_CLOBBER)          /** >|  */
        exec_redir_clobber(redir, fd);
    else if (redir->type == REDIR_READ_WRITE)       /** <>  */
        exec_redir_readwrite(redir, fd);
}

int set_redir(s_ast_redirection_list *redir)
{
    int fd = 0;

    while (redir)
    {
        exec_redir_type(redir, fd);
        redir = redir->next;
    }
    return 0;
}
