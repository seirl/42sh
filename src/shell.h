#ifndef SHELL_H
# define SHELL_H

typedef int (*f_handler)(char **);

struct builtin_entry
{
    char *name;
    f_handler callback;
};
typedef struct builtin_entry s_builtin_entry;

typedef struct shell
{
    s_builtin_entry *builtins;
    unsigned int built_count;
    char **curr_argv;
    int status;
} s_shell;

#endif /* !SHELL_H */
