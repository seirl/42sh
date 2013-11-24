#ifndef MACROS_H
# define MACROS_H

# include <stdio.h>
# include <assert.h>

# define VERSION "0.8"
# define PROGNAME "42sh"

# define RET_WITH(Ret, Msg, ...)                    \
    return fprintf(stderr, Msg, __VA_ARGS__), Ret;

#endif /* !MACROS_H */
