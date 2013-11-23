#ifndef EXEC_H
# define EXEC_H

# define _POSIX_SOURCE
# define _GNU_SOURCE

# include <fcntl.h>
# include <unistd.h>
# include <assert.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <assert.h>

# include "ast.h"
# include "string_utils.h"
# include "shell.h"

struct redir_context
{
    int old_fd;
    int tmp_fd;
};
typedef struct redir_context s_redir_context;

typedef struct pipe
{
    int nb_proc;
    int next_proc;
    pid_t *process;
} s_pipe;

// Exec -----------------------------------------------------------------------

/** @brief Execute a simple command */
void exec_simple_cmd(s_shell *shell, s_ast_simple_cmd *cmd);
/** @brief Execute an assignment command */
void exec_assignment(s_shell *shell, s_ast_assignment *assign);
/** @brief Execute an argument array */
void exec_argv(char **argv);
/** @brief Execute a program from a argv, a context list and a prefix list */
int exec_prog(s_shell *shell, char **cmd_argv,
              s_redir_context **contexts,
              s_ast_prefix *prefixes);
/** @brief Execute a prefix list */
void exec_prefixes(s_shell *shell, s_ast_prefix *prefix);
/** @brief Execute an element list */
void exec_elements(s_shell *shell, s_ast_element *elt);
/** @brief Set IO_NUMBER to its default value
**
** if no input is given by the user
*/
void set_default_io_number(s_ast_redirection_list *redir);
/** @brief Execute an 'if' command */
void exec_if(s_shell *shell, s_ast_if *if_cmd);
/** @brief Execute a 'while' command */
void exec_while(s_shell *shell, s_ast_while *while_cmd);
/** @brief Execute a 'until' command */
void exec_until(s_shell *shell, s_ast_until *until_cmd);
/** @brief Execute a 'for' command */
void exec_for(s_shell *shell, s_ast_for *for_cmd);
/** @brief Execute a shell command */
void exec_shell_cmd(s_shell *shell, s_ast_shell_cmd *shell_cmd);
/** @brief Execute a shell command AST node */
void exec_shell_cmd_node(s_shell *shell, s_ast_shell_cmd *shell_cmd,
                         s_ast_redirection_list *redir);
/** @brief Execute a function declaration */
void exec_fund_dec(s_shell *shell, s_ast_funcdec *funcdec);
/** @brief Execute a function declaration AST node */
void exec_funcdec_node(s_shell *shell, s_ast_funcdec *funcdec,
                       s_ast_redirection_list *redir);
/** @brief Execute a command AST node */
void exec_cmd_node(s_shell *shell, s_ast_cmd *node);
/** @brief Execute an 'and' AST node */
void exec_and_node(s_shell *shell, s_ast_and_or *node);
/** @brief Execute an 'or' AST node */
void exec_or_node(s_shell *shell, s_ast_and_or *node);
/** @brief Execute a generic 'or/and' AST node */
void exec_andor_node(s_shell *shell, s_ast_and_or *node);
/** @brief Return the number of piped commandes in a pipeline */
int pipe_cmd_count(s_ast_pipeline *node);
/** @brief Return a command array from a pipeline */
s_ast_cmd **pipe_cmd_array(s_ast_pipeline *node, int len);
/** @brief Set a pipe IO values */
void exec_pipe_setio(int pipe[2], int io);
/** @brief Execute a pipeline */
int exec_pipe(s_shell *shell, s_pipe *pipe,
              s_ast_cmd **pipe_cmds,
              int len);
/** @brief Execute a pipeline AST node */
void exec_pipe_node(s_shell *shell, s_ast_pipeline *node);
/** @brief Execute an AST node list */
void exec_ast_list(s_shell *shell, s_ast_list *list);
/** @brief Execute an AST input */
void exec_ast_input(s_shell *shell, s_ast_input *ast);

// Elements -------------------------------------------------------------------

/** @brief Return the length of an element list */
int element_list_len(s_ast_element *elt);
/** @brief Convert an element list to an argument array */
char **elements_to_argv(s_shell *shell, s_ast_element *element, int len);

// Redirections ---------------------------------------------------------------

/** @brief Execute the redirections existing in an element list */
s_redir_context **exec_elements_redir(s_shell *shell, s_ast_element *elt);
/** @brief Execute a redirection list */
s_redir_context *exec_redirection(s_shell *shell,
                                  s_ast_redirection_list *redir);
/** @brief Return the length of a redirection list */
int redir_list_len(s_ast_redirection_list *redir);
/** @brief Save a redirection context */
s_redir_context *save_redir_context(s_ast_redirection_list *redir);
/** @brief Translates a word to a file descriptor if possible */
int word_to_fd(s_string *str);
/** @brief Apply a redirection list */
int set_redir(s_shell *shell, s_ast_redirection_list *redir);
/** @brief Restore a redirection context */
void restore_redir_context(s_redir_context *context);
/** @brief Restore a redirection context list */
void restore_redir_contexts(s_redir_context **contexts);

// Expansion stubs ------------------------------------------------------------

/** @brief Return the length of a compound word */
int compound_word_len(s_ast_compound_word *word);
/** @brief Expand a word to a string */
s_string *expand_word(s_ast_word *word);
/** @brief Expand a compound list to a string */
s_string *expand_compound(s_shell *shell, s_ast_compound_word *word);
/** @brief Return an argument array from a compound list */
char **compword_to_argv(s_ast_compound_word *word, int len);

#endif /* !EXEC_H */
