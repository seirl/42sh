#ifndef AST_TYPES_H
# define AST_TYPES_H

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
# include "smalloc.h"

struct ast_node;

enum ast_node_type
{
    PIPE,
    CMD,
    FUNC,
    IF,
    FOR,
    WHILE,
    CASE,
    AND,
    OR
};

struct cmd_node
{
    char **prefix;
    char **argv;
};

struct binary_node
{
    struct ast_node *right;
    struct ast_node *left;
};

struct func_node
{
    char *name;
    struct ast_node *body;
};

struct if_node
{
    struct ast_node *cond;
    struct ast_node *if_true;
    struct ast_node *if_false;
};

struct while_node
{
    struct ast_node *cond;
    struct ast_node *statement;
}

struct for_node
{
    char *var;
    /* char ** or int * values */
    struct ast_node *statement;
};

#if 0
/* Redirection.. ?! */

enum redirect_type
{
    L_TO_R_S // Left to right, simple: >
    R_TO_L_S // Right to left, simple: <
    L_TO_R_D // Left to right, double: >>
    R_TO_L_D // Right to left, double: <<
    ...
}

struct redirect_node
{
    int *fd
    enum redirect_type type;
    ... words, ..?
}
#endif

union ast_node_child
{
    struct cmd_node cmd;
    struct binary_node pipe;
    struct func_node func;
};

struct ast_node
{
    enum ast_node_type type;
    union ast_node_child next;
};

typedef struct pipe
{
    int nb_proc;
    int next_proc;
    pid_t *process;
} s_pipe;

typedef enum side
{
    RIGHT,
    LEFT
} e_side;

void exec_pipe_node(struct binary_node *node);
void exec_cmd_node(struct cmd_node *cmd);
void exec_node(struct ast_node *node);

#endif /* !AST_TYPES_H */
