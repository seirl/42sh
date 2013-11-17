#ifndef 42SH_H
# define 42SH_H

HASHTBL(char *, s_ast_shell_cmd *, 42sh_funcs, null_free, null_free);
HASHTBL(char *, char *, 42sh_vars, null_free, null_free);
HASHTBL(char *, void *(handler)(), 42sh_builtins, null_free, null_free);

typedef struct 42sh
{
    s_42sh_funcs *funcs;
    s_42sh_vars *vars;
    s_42sh_builtins *builtins;
    int status;
} s_42sh;

#endif /* !42SH_H */
