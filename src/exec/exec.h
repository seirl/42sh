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
# include "smalloc.h"
# include "hashtbl.h"
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
extern s_shell shell;

s_string *expand_word(s_ast_word *word);
s_string *expand_compound(s_ast_compound_word *word);
void variable_add(s_string *var_name, s_string *var_value);
void function_add(s_ast_shell_cmd *cmd, s_string *var_value);
void exec_assignment(s_ast_assignment *assign);
int redir_list_len(s_ast_redirection_list *redir);
s_redir_context *save_redir_context(s_ast_redirection_list *redir);
int word_to_fd(s_string *str);
int set_redir(s_ast_redirection_list *redir);
void restore_redir_context(s_redir_context *context);
void exec_redirection(s_ast_redirection_list *redir);
int compound_word_len(s_ast_compound_word *word);
char **compword_to_argv(s_ast_compound_word *word, int len);
void exec_argv(char **argv);
int exec_program(char **cmd_argv, s_ast_prefix *prefixes);
void exec_cmd_word(s_ast_compound_word *word);
void exec_prefixes(s_ast_prefix *prefix);
void exec_elements(s_ast_element *elt);
void exec_simple_cmd(s_ast_simple_cmd *cmd);
void exec_if(s_ast_if *if_cmd);
void exec_while(s_ast_while *while_cmd);
void exec_until(s_ast_until *until_cmd);
void exec_for(s_ast_for *for_cmd);
void exec_shell_cmd(s_ast_shell_cmd *shell_cmd);
void exec_shell_cmd_node(s_ast_shell_cmd *shell_cmd,
                         s_ast_redirection_list *redir);
void exec_fund_dec(s_ast_funcdec *funcdec);
void exec_funcdec_node(s_ast_funcdec *funcdec,
                         s_ast_redirection_list *redir);
void exec_cmd_node(s_ast_cmd *node);
void exec_and_node(s_ast_and_or *node);
void exec_or_node(s_ast_and_or *node);
void exec_andor_node(s_ast_and_or *node);
int pipe_cmd_count(s_ast_pipeline *node);
s_ast_cmd **pipe_cmd_array(s_ast_pipeline *node, int len);
void exec_pipe_setio(int pipe[2], int io);
void pipe_child_job(int cmd_index,
                    int max_index,
                    int curr_pipe[2],
                    int old_pipe[2]);
void close_pipe(int pipe[2]);
int exec_pipe(s_pipe *pipe,
              s_ast_cmd **pipe_cmds,
              int len);
void exec_pipe_node(s_ast_pipeline *node);
void exec_ast_list(s_ast_list *list);
void exec_ast_input(s_ast_input *ast);

#endif /* !EXEC_H */
