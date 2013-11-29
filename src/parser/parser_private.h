#ifndef PARSER_PRIVATE_H
# define PARSER_PRIVATE_H

# include "parser.h"
# include "token.h"
# include "ast.h"

struct parser
{
    s_lexer *lexer;
    e_parser_status status;
};

// Rules ----------------------------------------------------------------------
/** @brief Parse rule list. */
s_ast_list *parse_rule_list(s_parser *parser);
/** @brief Parse rule and_or. */
s_ast_and_or *parse_rule_and_or(s_parser *parser);
/** @brief Parse rule pipeline. */
s_ast_pipeline *parse_rule_pipeline(s_parser *parser, int first);
/** @brief Parse rule command. */
s_ast_cmd *parse_rule_command(s_parser *parser);
/** @brief Parse rule simple command. */
s_ast_simple_cmd *parse_rule_simple_command(s_parser *parser);
/** @brief Parse rule shell command. */
s_ast_shell_cmd *parse_rule_shell_command(s_parser *parser);
/** @brief Parse rule funcdec. */
s_ast_funcdec *parse_rule_funcdec(s_parser *parser);
/** @brief Parse rule redirection. */
s_ast_redirection_list *parse_rule_redirection(s_parser *parser);
/** @brief Parse rule prefix. */
s_ast_prefix *parse_rule_prefix(s_parser *parser);
/** @brief Parse rule elements. */
s_ast_element *parse_rule_elements(s_parser *parser);
/** @brief Parse rule compound_list. */
s_ast_list *parse_rule_compound_list(s_parser *parser, int first);
/** @brief Parse rule for. */
s_ast_for *parse_rule_for(s_parser *parser);
/** @brief Parse rule while. */
s_ast_while *parse_rule_while(s_parser *parser);
/** @brief Parse rule until. */
s_ast_until *parse_rule_until(s_parser *parser);
/** @brief Parse rule case. */
s_ast_case *parse_rule_case(s_parser *parser);
/** @brief Parse rule if. */
s_ast_if *parse_rule_if(s_parser *parser);
/** @brief Parse rule else. */
s_ast_else *parse_rule_else(s_parser *parser);
/** @brief Parse rule do_group. */
s_ast_list *parse_rule_do_group(s_parser *parser);
/** @brief Parse rule case_clause. */
s_ast_case_item *parse_rule_case_clause(s_parser *parser);
/** @brief Parse rule case_item. */
s_ast_case_item *parse_rule_case_item(s_parser *parser);

// Bonus ----------------------------------------------------------------------
/** @brief Parse rule braces. */
s_ast_compound_word *parse_rule_braces(s_parser *parser);

// Expect ---------------------------------------------------------------------
/** @brief Grab as many newlines as you can! */
void parse_expect_newlines(s_parser *parser);
/** @brief Expect a token. */
int parse_expect_token(s_parser *parser, e_token_type type);
/** @brief Expect '\n' or ';' . */
int parse_expect_newline_or_semi(s_parser *parser);

// Utils ----------------------------------------------------------------------
/** @brief Merge words. */
s_ast_compound_word *parse_compound_word(s_parser *parser);
/** @brief Handle assignment. */
s_ast_assignment *parse_assignment(s_parser *parser);
/** @brief Handle heredoc body. */
int maybe_parse_heredoc(s_parser *parser, s_ast_cmd *cmd);
/** @brief Handle heredoc body. */
void parse_heredoc_here(s_parser *parser, s_ast_cmd *cmd);
/** @brief Create a s_ast_word from any token. */
s_ast_word *word_of_token(s_token *tok);

void parser_shift_token(s_parser *parser);
void parser_shift_word(s_parser *parser);
int next_word_is_res_word(s_parser *parser);

#endif /* !PARSER_PRIVATE_H */
