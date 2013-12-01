#include "exec.h"
#include "xsyscall.h"

static void exec_redir_write(s_shell *shell,
                             s_ast_redirection_list *redir,
                             int fd,
                             int clobber)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf,
                   O_CREAT | O_WRONLY | O_TRUNC,
                   0644)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    if (!clobber)
    {
        fcntl(redir->io->io_number, F_GETFD);
        fcntl(redir->io->io_number, F_DUPFD, 10);
        fcntl(redir->io->io_number, F_GETFD);
        fcntl(10, F_SETFD, FD_CLOEXEC);
        dup2(fd, redir->io->io_number);
        XCLOSE(fd);
        string_free(filename);
        return;
    }
    string_free(filename);
    fcntl(redir->io->io_number, F_DUPFD, 10);
    XCLOSE(redir->io->io_number);
    fcntl(10, F_SETFD, FD_CLOEXEC);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_read(s_shell *shell,
                            s_ast_redirection_list *redir,
                            int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    s_string *filename = expand_compound(shell, redir->word);
    if ((fd = open(filename->buf, O_RDONLY)) == -1)
        fprintf(stderr, "Cannot open file: %s.\n", filename->buf);
    string_free(filename);
    fcntl(redir->io->io_number, F_DUPFD, 10);
    XCLOSE(0);
    fcntl(10, F_SETFD, FD_CLOEXEC);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}

static void exec_redir_heredoc(s_ast_redirection_list *redir)
{
    int fildes[2];
    int i = 0;
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    if (!redir->heredoc || !redir->heredoc->heredoc)
    {
        fprintf(stderr, "here-document command failed\n");
        return;
    }
    else
    {
        fildes[0] = 3;
        fildes[1] = 4;
        i += pipe(fildes);
        i += write(4, redir->heredoc->heredoc->buf,
                   redir->heredoc->heredoc->len);
        XCLOSE(4);
        fcntl(0, F_DUPFD, 10);
        XCLOSE(0);
        fcntl(10, F_SETFD, FD_CLOEXEC);
        dup2(3, 0);
        XCLOSE(3);
    }
}

static void exec_redir_dupout(s_shell *shell,
                              s_ast_redirection_list *redir,
                              int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 1;
    s_string *filename = expand_compound(shell, redir->word);
    fd = word_dig_to_fd(filename);
    if (fd == -1)
        LOG(WARN,
            "42sh: %s: Syntax error: Bad fd number. Redirection ignored\n",
            filename->buf);
    else if (fd == -2)
        XCLOSE(fd); /* FIXME error handling */
    else
    {
        string_free(filename);
        fcntl(redir->io->io_number, F_GETFD);
        fcntl(redir->io->io_number, F_DUPFD, 10);
        fcntl(redir->io->io_number, F_GETFD);
        fcntl(10, F_SETFD, FD_CLOEXEC);
        dup2(fd, redir->io->io_number);
        fcntl(fd, F_GETFD);
    }
}

static void exec_redir_dupin(s_shell *shell,
                             s_ast_redirection_list *redir,
                             int fd)
{
    if (!redir->io || redir->io->io_number == -2)
        redir->io->io_number = 0;
    s_string *filename = expand_compound(shell, redir->word);
    fd = word_dig_to_fd(filename);
    if (fd == -1)
        LOG(WARN,
            "42sh: %s: Syntax error: Bad fd number. Redirection ignored\n",
            filename->buf);
    else if (fd == -2)
        XCLOSE(fd); /* FIXME error handling */
    else
    {
        string_free(filename);
        dup2(fd, redir->io->io_number);
    }
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
    else
    {
        string_free(filename);
        dup2(fd, redir->io->io_number);
        XCLOSE(fd);
    }
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
    else
    {
        fcntl(redir->io->io_number, F_DUPFD, 10);
        XCLOSE(redir->io->io_number);
        fcntl(10, F_SETFD, FD_CLOEXEC);
        dup2(fd, redir->io->io_number);
        XCLOSE(fd);
    }
    string_free(filename);
}

static void exec_redir_type(s_shell *shell, s_ast_redirection_list *redir,
                            int fd)
{
    if (redir->type == REDIR_WRITE)                 /** >   */
        exec_redir_write(shell, redir, fd, 0);
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
        exec_redir_write(shell, redir, fd, 1);
    else if (redir->type == REDIR_READ_WRITE)       /** <>  */
        exec_redir_readwrite(shell, redir, fd);
}

static void revert_set_redir(s_shell *shell,
                             s_ast_redirection_list *redir,
                             int fd)
{
    if (!redir)
        return;
    revert_set_redir(shell, redir->next, fd);
    set_default_io_number(redir);
    if (!shell->tmp_fd)
        shell->tmp_fd = 1;
    exec_redir_type(shell, redir, fd);
}

int set_redir(s_shell *shell, s_ast_redirection_list *redir)
{
    int fd = 0;
    revert_set_redir(shell, redir, fd);
    return 0;
}
