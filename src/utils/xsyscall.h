#ifndef XSYSCALL_H
# define XSYSCALL_H

# include <stdio.h>
# include <unistd.h>

# include "log.h"

# define XCLOSE(Fd)                         \
    do {                                    \
        int _ret = close(Fd);               \
        if (_ret == -1)                     \
            LOG(ERROR, "close: %d", Fd);    \
    } while (0)

# define XPIPE(Fds)                          \
    do {                                     \
        int _ret = pipe(Fds);                \
        if (_ret < 0)                        \
            LOG(ERROR, "pipe failed", NULL); \
    } while (0)

# define XWRITE(Fd, Str, Len)                 \
    do {                                      \
        int _ret = write(Fd, Str, Len);       \
        if (_ret < 0)                         \
            LOG(ERROR, "write failed", NULL); \
    } while (0)

#endif /* !XSYSCALL_H */
