#ifndef SHELL_H
# define SHELL_H

HASHTBL(s_ast_shell_cmd *, char *, shell_funcs);
HASHTBL(s_string *, s_string *, shell_vars);
typedef int (*handler)(char **);

struct builtin_entry
{
    char *name;
    handler callback;
};
typedef struct builtin_entry s_builtin_entry;

typedef struct shell
{
    s_shell_funcs *funcs;
    unsigned int fun_count;
    s_shell_vars *vars;
    unsigned int var_count;
    s_builtin_entry *builtins;
    unsigned int built_count;
    int status;
} s_shell;

#endif /* !SHELL_H */
