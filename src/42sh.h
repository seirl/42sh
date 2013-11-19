#ifndef 42SH_H
# define 42SH_H

HASHTBL(s_string *, s_ast_shell_cmd *, 42sh_funcs);
HASHTBL(s_string *, s_string *, 42sh_vars);
HASHTBL(char *, void *(handler)(), 42sh_builtins);

typedef struct 42sh
{
    s_42sh_funcs *funcs;
    unsigned int fun_count;
    s_42sh_vars *vars;
    unsigned int var_count;
    s_42sh_builtins *builtins;
    unsigned int built_count;
    int status;
} s_42sh;

#endif /* !42SH_H */
