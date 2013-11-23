#include "exec.h"
#include "xsyscall.h"

static void exec_redir_write(s_shell *shell, s_ast_redirection_list *redir,
                             int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_TRUNC,
                   0644)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_read(s_shell *shell, s_ast_redirection_list *redir,
                           int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf, O_RDONLY)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_heredoc(s_ast_redirection_list *redir)
{
    int unused = 0;
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    if (!redir->heredoc || !redir->heredoc->heredoc)
    {
        fprintf(stderr, "here-document command failed\n");
        return;
    }
    unused += write(redir->io->io_number,
                    redir->heredoc->heredoc->buf,
                    redir->heredoc->heredoc->len);
}

static void exec_redir_dupout(s_shell *shell, s_ast_redirection_list *redir,
                              int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    fd = word_to_fd(filename);
    if (fd == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    else if (fd == -2)
        XCLOSE(fd); /* FIXME error handling */
    dup2(fd, redir->io->io_number);
}

static void exec_redir_dupin(s_shell *shell, s_ast_redirection_list *redir,
                            int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    s_string *filename = expand_compound(shell, redir->word);
    fd = word_to_fd(filename);
    if (fd == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    else if (fd == -2)
        XCLOSE(fd); /* FIXME error handling */
    dup2(fd, redir->io->io_number);
}

static void exec_redir_clobber(s_shell *shell, s_ast_redirection_list *redir,
                              int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_TRUNC,
                   0644)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_readwrite(s_shell *shell, s_ast_redirection_list *redir,
                                 int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_RDWR | O_APPEND,
                   666)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_writeup(s_shell *shell, s_ast_redirection_list *redir,
                               int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_RDWR | O_TRUNC,
                   666)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_type(s_shell *shell, s_ast_redirection_list *redir,
                            int fd)
{
    if (redir->type == REDIR_WRITE)                 /** >   */
        exec_redir_write(shell, redir, fd);
    else if (redir->type == REDIR_WRITE_UPDATE)     /** >>  */
        exec_redir_writeup(shell, redir, fd);
    else if (redir->type == REDIR_READ)             /** <   */
        exec_redir_read(shell, redir, fd);
    else if (redir->type == REDIR_HEREDOC)          /** <<  */
        exec_redir_heredoc(redir);
    else if (redir->type == REDIR_HEREDOC_STRIP)    /** <<- */
        exec_redir_heredoc(redir);
    else if (redir->type == REDIR_DUPLICATE_INPUT)  /** <&  */
        exec_redir_dupin(shell, redir, fd);
    else if (redir->type == REDIR_DUPLICATE_OUTPUT) /** >&  */
        exec_redir_dupout(shell, redir, fd);
    else if (redir->type == REDIR_CLOBBER)          /** >|  */
        exec_redir_clobber(shell, redir, fd);
    else if (redir->type == REDIR_READ_WRITE)       /** <>  */
        exec_redir_readwrite(shell, redir, fd);
}

int set_redir(s_shell *shell, s_ast_redirection_list *redir)
{
    int fd = 0;

    while (redir)
    {
        set_default_io_number(redir);
        exec_redir_type(shell, redir, fd);
        redir = redir->next;
    }
    return 0;
}
