#ifndef SHELL_H
# define SHELL_H

struct parser;

enum shell_state
{
    SHELL_STOP,
    SHELL_REPEAT,
};
typedef enum shell_state e_shell_state;

typedef struct shell s_shell;

/**
** @brief Create a new shell.
*/
s_shell *shell_new(void);

/**
** @brief Setup the shell for usage.
*/
void shell_setup(s_shell *shell,
                 struct parser *parser);

/**
** @brief Delete the shell.
*/
void shell_delete(s_shell *shell);

/**
** @brief Start the read-eval loop.
*/
int shell_loop(s_shell *shell);

#endif /* !SHELL_H */
