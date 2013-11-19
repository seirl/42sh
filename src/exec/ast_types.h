#ifndef AST_TYPES_H
# define AST_TYPES_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>

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
    REDIR,
    BANG,
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
};

struct for_node
{
    char *var;
    char **val;
    struct ast_node *statement;
};

struct case_switch
{
    char **values;
    struct ast_node *statement;
    struct case_switch *next_case;
};

struct case_node
{
    char *var;
    struct case_switch *first_case;
};

enum redirect_type
{
    L_TO_R_S, //! @brief Left to right, simple: >
    R_TO_L_S, //! @brief Right to left, simple: <
    L_TO_R_D, //! @brief Left to right, double: >>
    R_TO_L_D, //! @brief Right to left, double: <<
    L_TO_R_SA, //! @brief Left to right, simple, and: >&
    R_TO_L_SA, //! @brief Right to left, simple, and: <&
    L_TO_R_SO, //! @brief Left to right, simple, or: >|
    R_TO_L_DD, //! @brief Right to left, double, dash: <<-
    LR_TO_RL //! @brief Bidirectionnal: <>
};

struct redir_node
{
    enum redirect_type type;
    int *fd;
};

union ast_node_child
{
    struct cmd_node cmd_n;
    struct binary_node pipe_n;
    struct func_node func_n;
    struct if_node if_n;
    struct for_node for_n;
    struct case_node case_n;
    struct while_node while_n;
    struct binary_node bang_n;
    struct binary_node or_n;
    struct binary_node and_n;
    struct redir_node redir_n;
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

extern int status;

#endif /* !AST_TYPES_H */
