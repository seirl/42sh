#ifndef SHELL_PRIVATE_H
# define SHELL_PRIVATE_H

# include "shell.h"

# include "builtins.h"
# include "hashtbl.h"
# include "history.h"
# include "parser.h"
# include "shopt.h"
# include "alias.h"

struct shell
{
    /** Not managed by us. */
    s_parser *parser;

    /** Managed by us. */
    s_builtins *builtins;
    size_t builtins_count;

    /** Managed by the env module. */
    s_hashtbl *env;

    /** Managed by the options module */
    s_shopt shopt[SHOPT_COUNT];

    /** Managed by us. */
    s_hashtbl *functions;

    /** Managed by the history module. */
    s_history *history;

    /** Managed by the lexer module. */
    s_hashtbl *aliases;

    char **curr_argv;

    /** Error code of the shell. */
    int status;

    /** Number of breaks left to do */
    unsigned int breaks;

    /** Status of the shell. */
    e_shell_state state;
};

#endif /* !SHELL_PRIVATE_H */
