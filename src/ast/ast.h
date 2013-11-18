#ifndef AST_H
# define AST_H

# include "string_utils.h"

// Forward declarations
typedef struct ast_list s_ast_list;
typedef struct ast_element s_ast_element;

enum ast_word_kind
{
    WORD,         /** a or 'a' or "a" */
    SUBSTS_SHELL, /** $(echo foo)     */
    SUBSTS_VAR,   /** $(#argv)        */
    EXPAND_VAR,   /** $a              */
};
typedef enum ast_word_kind e_ast_word_kind;

const char *word_kind_str[] =
{
    "WORD",
    "SUBSTS_SHELL",
    "SUBSTS_VAR",
    "EXPAND_VAR",
};

/**
** @brief A word.
*/
struct ast_word
{
    s_string *str;
    e_ast_word_kind kind;
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
    s_string *heredoc;
};
typedef struct ast_heredoc s_ast_heredoc;

struct ast_assignment
{
    s_string *name;
    s_ast_compound_word *value;
};
typedef struct ast_assignment s_ast_assignment;

enum ast_redirection_type
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
typedef enum ast_redirection_type e_ast_redirection_type;

const char *redirection_type_str[] =
{
    ">",
    ">>",
    "<",
    "<<",
    "<<-",
    "<&",
    ">&",
    ">|",
    "<>",
};

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

struct ast_redirection_list
{
    s_ast_io_number *io;
    s_ast_compound_word *word;
    s_ast_heredoc *heredoc;
    s_ast_compound_word *heredoc_delim;
    e_ast_redirection_type type;
    struct ast_redirection_list *next;
};
typedef struct ast_redirection_list s_ast_redirection_list;

/**
** @brief Prefixes
**
** @code
** prefixes: (ASSIGMENT_WORD | redirection)+
** @endcode
*/
struct ast_prefix
{
    s_ast_assignment *assignment;
    s_ast_redirection_list *redirection;
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
    s_ast_redirection_list *redirection;
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
    s_ast_list *elif_predicate;
    s_ast_list *elif_cmds;
    s_ast_list *else_cmds;
};
typedef struct ast_if s_ast_if;

struct ast_until
{
    s_ast_list *predicate;
    s_ast_list *cmds;
};
typedef struct ast_until s_ast_until;

struct ast_while
{
    s_ast_list *predicate;
    s_ast_list *cmds;
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
    s_ast_redirection_list *redirections; /** for shell_cmd of funcdec */
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
** and_or: pipeline (('&&'|'||') ('\n')* pipeline)*
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

// Functions ------------------------------------------------------------------
s_ast_and_or *ast_and_or_new(void);
s_ast_assignment *ast_assignment_new(void);
s_ast_cmd *ast_cmd_new(void);
s_ast_compound_word *ast_compound_word_new(void);
s_ast_element *ast_element_new(void);
s_ast_for *ast_for_new(void);
s_ast_if *ast_if_new(void);
s_ast_input *ast_input_new(void);
s_ast_io_number *ast_io_number_new(void);
s_ast_list *ast_list_new(void);
s_ast_pipeline *ast_pipeline_new(void);
s_ast_prefix *ast_prefix_new(void);
s_ast_redirection_list *ast_redirection_new(void);
s_ast_shell_cmd *ast_shell_cmd_new(void);
s_ast_simple_cmd *ast_simple_cmd_new(void);
s_ast_until *ast_until_new(void);
s_ast_while *ast_while_new(void);
s_ast_word *ast_word_new(void);
s_ast_word_list *ast_word_list_new(void);

void ast_and_or_delete(s_ast_and_or *ao);
void ast_assignment_delete(s_ast_assignment *ass);
void ast_case_delete(s_ast_case *casee);
void ast_cmd_delete(s_ast_cmd *cmd);
void ast_compound_word_delete(s_ast_compound_word *cw);
void ast_element_delete(s_ast_element *elem);
void ast_for_delete(s_ast_for *forr);
void ast_func_dec_delete(s_ast_funcdec *fd);
void ast_heredoc_delete(s_ast_heredoc *hd);
void ast_if_delete(s_ast_if *iff);
void ast_input_delete(s_ast_input *input);
void ast_io_number_delete(s_ast_io_number *io);
void ast_list_delete(s_ast_list *list);
void ast_pipeline_delete(s_ast_pipeline *pipeline);
void ast_prefix_delete(s_ast_prefix *pref);
void ast_redirection_list_delete(s_ast_redirection_list *redir);
void ast_shell_cmd_delete(s_ast_shell_cmd *cmd);
void ast_simple_cmd_delete(s_ast_simple_cmd *cmd);
void ast_until_delete(s_ast_until *until);
void ast_while_delete(s_ast_while *whilee);
void ast_word_delete(s_ast_word *word);
void ast_word_list_delete(s_ast_word_list *wl);

#endif /* !AST_H */
