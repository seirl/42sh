#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

typedef int (*f_handler)(char **);

struct builtins
{
    const char *name;
    f_handler callback;
};
typedef struct builtins s_builtins;

f_handler builtins_find(s_shell *shell, const char *name);

#endif /* !BUILTINS_H */
