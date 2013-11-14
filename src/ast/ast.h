#ifndef AST_H
# define AST_H

enum redirection_type
{
    REDIR_WRITE, /** > */
    REDIR_WRITE_UPDATE, /** >> */
    REDIR_READ, /** < */
    // ...
};
typedef enum redirection_type e_redirection_type;

struct ast_redirection
{
    e_redirection_type type;
    // TODO: s_ast_word to;
    // TODO: s_ast_heredoc to;
};
typedef struct ast_redirection s_ast_redirection;

struct ast_cmd_prefix
{
    // TODO: s_ast_assignement *assignement;
    s_ast_redirection *redirection;
    struct ast_cmd_prefix *next;
};
typedef struct ast_cmd_prefix s_ast_cmd_prefix;

struct ast_simple_cmd
{
    s_ast_cmd_prefix *prefixes;
    // TODO: s_ast_cmd_element *elements;
};
typedef struct ast_simple_cmd s_ast_simple_cmd;

enum ctrl_structure
{
    AST_FOR,
    AST_WHILE,
    AST_UNTIL,
    AST_CASE,
    AST_IF,
};
typedef enum ctrl_structure u_ctrl_structure;

struct ast_shell_cmd
{
    // TODO: compount_list_sameshell { }
    // TODO: compount_list_subshell ( )

    u_ctrl_structure ctrl_structure;
    // TODO: Add ctrl structures union ptr
};
typedef struct ast_shell_cmd s_ast_shell_cmd;

struct ast_funcdec
{
    s_string *name;
    s_ast_shell_cmd *shell_cmd;
};
typedef struct ast_funcdec s_ast_funcdec;

struct ast_cmd
{
    s_ast_simple_cmd *simple_cmd;
    s_ast_shell_cmd *shell_cmd;
    s_ast_funcdec *func_dec;
    s_ast_redirection *redirections; /** for shell_cmd of funcdec */
};
typedef struct ast_cmd s_ast_cmd;

/** @brief Pipeline
**
** pipeline : ['!'] command ('|' ('\n')* command)*
*/
struct ast_pipeline
{
    int inverted; /** ! cmd */
    s_ast_cmd *cmd;
    struct ast_pipeline *next; /** cmd | next */
};
typedef struct ast_pipeline s_ast_pipeline;

enum ast_pipeline_and_or
{
    AST_AND, /** && */
    AST_OR, /** || */
};
typedef enum ast_pipeline_and_or e_ast_pipeline_and_or;

/**
** @brief And or Or
**
** and_or: pipeline (('&&'|'|| ') ('\n')* pipeline)*
*/
struct ast_and_or
{
    e_ast_pipeline_and_or and_or;
    s_ast_pipeline *pipeline;
    struct ast_and_or *next;
};
typedef struct ast_and_or s_ast_and_or;

/**
** @brief List
**
** list: and_or (( ';'|'& ') and_or )* [';'|'&']
*/
struct ast_list
{
    int next_asynchronous; /** 1 if & else 0 */
    s_ast_and_or *and_or;
    struct ast_list *next;
};
typedef struct ast_list s_ast_list;

struct ast_input
{
    s_ast_list *list;
};
typedef struct ast_input s_ast_input;

#endif /* !AST_H */
