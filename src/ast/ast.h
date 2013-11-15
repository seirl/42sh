#ifndef AST_H
# define AST_H

# include "string_utils.h"

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

struct ast_prefix
{
    // TODO: s_ast_assignement *assignement;
    s_ast_redirection *redirection;
    struct ast_prefix *next;
};
typedef struct ast_prefix s_ast_prefix;

// XXX: Will be used to form ast_compound_word, and probably expansion hints
struct ast_word
{
    s_string *word;
};
typedef struct ast_word s_ast_word;

/**
** elements: (WORD | redirection)+
*/
struct ast_element
{
    s_ast_word *word;
    s_ast_redirection *redirection;
    struct ast_element *next;
};
typedef struct ast_element s_ast_element;

/**
** simple_command: (prefix)+
**          | (prefix)* elements
*/
struct ast_simple_cmd
{
    s_ast_prefix *prefixes;
    s_ast_element *elements;
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
typedef enum ctrl_structure e_ctrl_structure;

/**
** @brief Shell command
**
** shell_command: '{' compound_list '}'
**          | '(' compound_list ')'
**          | rule_for
**          | rule_while
**          | rule_until
**          | rule_case
**          | rule_if
*/
struct ast_shell_cmd
{
    // TODO: compount_list_sameshell { }
    // TODO: compount_list_subshell ( )

    e_ctrl_structure ctrl_structure;
    // TODO: Add ctrl structures union ptr
};
typedef struct ast_shell_cmd s_ast_shell_cmd;

/**
** @breif Function definition
**
** funcdec: [' function '] WORD '(' ')' ('\n')* shell_command
*/
struct ast_funcdec
{
    s_string *name;
    s_ast_shell_cmd *shell_cmd;
};
typedef struct ast_funcdec s_ast_funcdec;

/**
** @brief Command
**
** command: simple_command
**          | shell_command (redirection)*
**          | funcdec (redirection)*
*/
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
** pipeline: ['!'] command ('|' ('\n')* command)*
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
    AST_CMD_AND, /** && */
    AST_CMD_OR, /** || */
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

/**
** @brief input
**
** input: list ('\n'|EOF) | \n | EOF
*/
struct ast_input
{
    s_ast_list *list;
};
typedef struct ast_input s_ast_input;

#endif /* !AST_H */
