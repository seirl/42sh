#include "exec.h"
#include "log.h"
#include "smalloc.h"

int word_dig_to_fd(s_string *str)
{
    long fd = 0;
    char *endptr = NULL;

    if (str->len == 0)
        return -1;
    if ((string_index(str, 0) == '-') && (string_index(str, 1) == '\0'))
        return -2;
    fd = strtol(str->buf, &endptr, 10);
    if ((endptr != 0) && strcmp("", endptr))
        return -1;
    if (fd > 2)
        return -1;
    return fd;
}

void clobber_handler(s_shell *shell, s_ast_redirection_list *redir, int fd)
{
    if (!shell->tmp_fd)
        shell->tmp_fd = 1;
    fcntl(redir->io->io_number, F_DUPFD, 10);
    XCLOSE(redir->io->io_number);
    fcntl(10, F_SETFD, FD_CLOEXEC);
    dup2(fd, redir->io->io_number);
    XCLOSE(fd);
}
