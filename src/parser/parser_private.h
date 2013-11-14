#ifndef PARSER_PRIVATE_H
# define PARSER_PRIVATE_H

# include "parser.h"
# include "ast.h"

struct parser
{
    s_lexer *lexer;
};

/** @brief Parse rule input. */
s_ast_input *parse_rule_input(s_parser *parser);
/** @brief Parse rule list. */
s_ast_list *parse_rule_list(s_parser *parser);

#endif /* !PARSER_PRIVATE_H */
