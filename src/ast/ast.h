#ifndef AST_H
# define AST_H

# include "string_utils.h"

// Forward declarations
typedef struct ast_list s_ast_list;
typedef struct ast_element s_ast_element;

/**
** @brief A word.
*/
struct ast_word
{
    s_string *str;
};
typedef struct ast_word s_ast_word;

/**
** @brief A word composed of multiple tokens/words.
**
** Example: for'bar'
*/
struct ast_compound_word
{
    s_ast_word *word;
    struct ast_compound_word *next;
};
typedef struct ast_compound_word s_ast_compound_word;

struct ast_io_number
{
    unsigned io_number;
};
typedef struct ast_io_number s_ast_io_number;

struct ast_heredoc
{
    s_string heredoc;
};
typedef struct ast_heredoc s_ast_heredoc;

struct ast_assignment
{
    s_ast_word *word;
    s_ast_element *value;
};
typedef struct ast_assignment s_ast_assignment;

enum redirection_type
{
    REDIR_WRITE,            /** >   */
    REDIR_WRITE_UPDATE,     /** >>  */
    REDIR_READ,             /** <   */
    REDIR_HEREDOC,          /** <<  */
    REDIR_HEREDOC_STRIP,    /** <<- */
    REDIR_DUPLICATE_INPUT,  /** <&  */
    REDIR_DUPLICATE_OUTPUT, /** >&  */
    REDIR_CLOBBER,          /** >|  */
    REDIR_READ_WRITE,       /** <>  */
};
typedef enum redirection_type e_redirection_type;

/**
** @brief Redirections
**
** @code
** redirection:
**        [IONUMBER] '>' WORD
**      | [IONUMBER] '<' WORD
**      | [IONUMBER] '>>' WORD
**      | [IONUMBER] '<<' HEREDOC
**      | [IONUMBER] '<<-' HEREDOC
**      | [IONUMBER] '>&' WORD
**      | [IONUMBER] '<&' WORD
**      | [IONUMBER] '>|' WORD
**      | [IONUMBER] '<>' WORD
** @endcode
*/
struct ast_redirection
{
    s_ast_io_number *io;
    s_ast_word *word;
    s_ast_heredoc *heredoc;
    e_redirection_type type;
};
typedef struct ast_redirection s_ast_redirection;

/**
** @brief Prefixes
**
** @code
** prefixes: (ASSIGMENT_WORD | redirection)+
** @endcode
*/
struct ast_prefix
{
    s_ast_assignment *assignement;
    s_ast_redirection *redirection;
    struct ast_prefix *next;
};
typedef struct ast_prefix s_ast_prefix;

/**
** @brief Elements
**
** @code
** elements: (WORD | redirection)+
** @endcode
*/
struct ast_element
{
    s_ast_compound_word *word;
    s_ast_redirection *redirection;
    // TODO: add T_SHELL_SUBST $()
    // TODO: add T_VAR_EXP ${}
    // TODO: add T_MATH_EXP $(())
    struct ast_element *next;
};
// typedef forward declared

/**
** @brief Simple command
**
** @code
** simple_command: (prefix)+
**          | (prefix)* elements
** @endcode
*/
struct ast_simple_cmd
{
    s_ast_prefix *prefixes;
    s_ast_element *elements;
};
typedef struct ast_simple_cmd s_ast_simple_cmd;

struct ast_word_list
{
    s_ast_compound_word *word;
    struct ast_word_list *next;
};
typedef struct ast_word_list s_ast_word_list;

struct ast_case_item
{
    s_ast_word_list *matches;
    s_ast_list *cmd_list;
};
typedef struct ast_case_item s_ast_case_item;

struct ast_case
{
    s_ast_compound_word *word;
    s_ast_case_item *clauses;
};
typedef struct ast_case s_ast_case;

/**
** @brief For
**
** @code
** rule_for:
**          'for' WORD ('\n')* ['in' (WORD)* (';'|'\n') ('\n')*] do_group
** @endcode
*/
struct ast_for
{
    s_ast_word *identifier; /** XXX: add s_ast_name? */
    s_ast_word_list *values;
    s_ast_list *cmd_list;
};
typedef struct ast_for s_ast_for;

/**
** @brief If
**
** @code
** rule_if:
**          'if' compound_list 'then' compound_list [else_clause] 'fi'
** @endcode
*/
struct ast_if
{
    s_ast_list *predicate;
    s_ast_list *then_cmds;
    s_ast_list *elif_cmds;
    s_ast_list *else_cmds;
};
typedef struct ast_if s_ast_if;

struct ast_until
{
    s_ast_list *predicate;
    s_ast_list *cms;
};
typedef struct ast_until s_ast_until;

struct ast_while
{
    s_ast_list *predicate;
    s_ast_list *cms;
};
typedef struct ast_while s_ast_while;

enum ctrl_structure_kind
{
    AST_FOR,
    AST_WHILE,
    AST_UNTIL,
    AST_CASE,
    AST_IF,
};
typedef enum ctrl_structure_kind e_ctrl_structure_kind;

union ctrl_structure
{
    s_ast_case *ast_case;
    s_ast_for *ast_for;
    s_ast_if *ast_if;
    s_ast_until *ast_until;
    s_ast_while *ast_while;
};
typedef union ctrl_structure u_ctrl_structure;

/**
** @brief Shell command
**
** @code
** shell_command: '{' compound_list '}'
**          | '(' compound_list ')'
**          | rule_for
**          | rule_while
**          | rule_until
**          | rule_case
**          | rule_if
** @endcode
*/
struct ast_shell_cmd
{ 
    int subshell; /** if 1 then ( ) else { } */
    s_ast_list *cmd_list;

    e_ctrl_structure_kind ctrl_structure;
    u_ctrl_structure ctrl;
};
typedef struct ast_shell_cmd s_ast_shell_cmd;

/**
** @brief Function definition
**
** @code
** funcdec: ['function'] WORD '(' ')' ('\n')* shell_command
** @endcode
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
** @code
** command: simple_command
**          | shell_command (redirection)*
**          | funcdec (redirection)*
** @endcode
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
** @code
** pipeline: ['!'] command ('|' ('\n')* command)*
** @endcode
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
** @code
** and_or: pipeline (('&&'|'|| ') ('\n')* pipeline)*
** @endcode
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
** @code
** list: and_or (( ';'|'& ') and_or )* [';'|'&']
** @endcode
*/
struct ast_list
{
    int next_asynchronous; /** 1 if & else 0 */
    s_ast_and_or *and_or;
    struct ast_list *next;
};
// typedef forward declared

/**
** @brief input
**
** @code
** input: list ('\n'|EOF) | \n | EOF
** @endcode
*/
struct ast_input
{
    s_ast_list *list;
};
typedef struct ast_input s_ast_input;

#endif /* !AST_H */
