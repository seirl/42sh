#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int builtin_shopt(s_shell *shell, int argc, char *argv[]);
int builtin_alias(s_shell *shell, int argc, char *argv[]);
int builtin_unalias(s_shell *shell, int argc, char *argv[]);
int builtin_set(s_shell *shell, int argc, char *argv[]);
int builtin_unset(s_shell *shell, int argc, char *argv[]);
int builtin_echo(s_shell *shell, int argc, char *argv[]);
int builtin_exit(s_shell *shell, int argc, char *argv[]);
int builtin_exec(s_shell *shell, int argc, char *argv[]);
int builtin_source(s_shell *shell, int argc, char *argv[]);
int builtin_cd(s_shell *shell, int argc, char *argv[]);
int builtin_history(s_shell *shell, int argc, char *argv[]);
int builtin_break(s_shell *shell, int argc, char *argv[]);
int builtin_continue(s_shell *shell, int argc, char *argv[]);
int builtin_export(s_shell *shell, int argc, char *argv[]);


typedef int (*f_handler)(s_shell *shell, int argc, char *argv[]);

struct builtins
{
    const char *name;
    f_handler callback;
};
typedef struct builtins s_builtins;

void builtins_create(s_shell *shell);
f_handler builtins_find(s_shell *shell, const char *name);
void builtins_free(s_shell *shell);

#endif /* !BUILTINS_H */
