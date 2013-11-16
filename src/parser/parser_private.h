#ifndef PARSER_PRIVATE_H
# define PARSER_PRIVATE_H

# include "parser.h"
# include "token.h"
# include "ast.h"

struct parser
{
    s_lexer *lexer;
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
/** @brief Parse rule prefix. */
s_ast_prefix *parse_rule_prefix(s_parser *parser);
/** @brief Parse rule elements. */
s_ast_element *parse_rule_elements(s_parser *parser);

// Expect ---------------------------------------------------------------------
/** @brief Grab as many newlines as you can! */
void parse_expect_newlines(s_parser *parser);
s_ast_compound_word *parse_compound_word(s_parser *parser);

// Parse tools ----------------------------------------------------------------
s_ast_word *word_of_token(s_token *tok);
void parser_shift_token(s_parser *parser);
void parser_shift_word(s_parser *parser);

#endif /* !PARSER_PRIVATE_H */
