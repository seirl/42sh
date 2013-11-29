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

#endif /* !XSYSCALL_H */
