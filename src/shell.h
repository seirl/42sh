#ifndef SHELL_H
# define SHELL_H

HASHTBL(char *, s_ast_shell_cmd *, shell_funcs, null_free, null_free);
HASHTBL(s_string *, s_string *, shell_vars, null_free, null_free);
HASHTBL(char *, void *(handler)(), shell_builtins, null_free, null_free);

typedef struct shell
{
    s_shell_funcs *funcs;
    unsigned int fun_count;
    s_shell_vars *vars;
    unsigned int var_count;
    s_shell_builtins *builtins;
    unsigned int built_count;
    int status;
} s_shell;

#endif /* !SHELL_H */
