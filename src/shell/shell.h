#ifndef SHELL_H
# define SHELL_H

struct parser;

enum shell_status
{
    SHELL_OK,
    SHELL_STOP,
};
typedef enum shell_status e_shell_status;

enum shell_state
{
    SHELL_STOP_ON_ERROR,
    SHELL_CONTINUE_ON_ERROR,
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
** @brief Create a duplicate of the shell.
*/
s_shell *shell_duplicate(s_shell *shell);

/**
** @brief Delete a duplicate of the shell.
*/
void shell_delete_duplicate(s_shell *shell);

/**
** @brief Start the read-eval loop.
*/
e_shell_status shell_loop(s_shell *shell);

/**
** @brief Returns the status of the shell.
*/
int shell_status(s_shell *shell);

/**
** @brief Set the return value and $?.
*/
void shell_status_set(s_shell *shell, int status);

#endif /* !SHELL_H */
