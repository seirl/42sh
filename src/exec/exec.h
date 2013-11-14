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
# include "ast_types.h"
# include "../utils/smalloc.h"

void exec_pipe_node(struct binary_node *node);
void exec_cmd_node(struct cmd_node *cmd);
void exec_node(struct ast_node *node);
void exec_bang_node(struct binary_node *node);
void exec_if_node(struct if_node *node);
void exec_for_node(struct for_node *node);
void exec_while_node(struct while_node *node);
void exec_case_node(struct case_node *node);
void exec_func_node(struct func_node *node);
void exec_or_node(struct binary_node *node);
void exec_and_node(struct binary_node *node);
void exec_redir_node(struct redir_node *node);

#endif /* !EXEC_H */
