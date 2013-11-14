#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

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

#endif /* !PARSER_H */
