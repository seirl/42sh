#include "parser_private.h"
#include "log.h"

s_ast_input *parse_rule_input(s_parser *parser)
{
    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_NEWLINE
        || tok->type == T_EOF)
    {
        token_free(tok);
        parser_shift_token(parser);
        return ast_input_new(); /** nothing is a valid input */
    }
    token_free(tok);

    s_ast_list *list;
    if (!(list = parse_rule_list(parser)))
        return NULL; /** Could not parse */
    s_ast_input *ast = ast_input_new();
    ast->list = list;

    tok = lex_token(parser->lexer);
    if (!(tok->type == T_NEWLINE
        || tok->type == T_EOF))
        LOG(ERROR, "parser: unexpected 'input' token.", NULL);
    token_free(tok);

    return ast;
}
