#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "ast.h"

typedef struct parser s_parser;

/**
** @brief Create a new parser.
**
** Transfer ownership of the lexer to the new parser.
*/
s_parser *parser_create(s_lexer *lexer);

/**
** @brief Delete a parser and the associated lexer.
*/
void parser_delete(s_parser *parser);

/** @brief Parse rule input. */
s_ast_input *parse_rule_input(s_parser *parser);

/** @brief Parse done? */
int parser_eof(s_parser *parser);

#endif /* !PARSER_H */
