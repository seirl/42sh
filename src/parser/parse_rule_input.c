#include "parser_private.h"
#include "smalloc.h"
#include "log.h"

static s_ast_input *ast_input_new(void)
{
    s_ast_input *ret;
    ret = smalloc(sizeof (s_ast_input));
    ret->list = NULL;
    return ret;
}

s_ast_input *parse_rule_input(s_parser *parser)
{
    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_NEWLINE
        || tok->type == T_EOF)
    {
        sfree(lex_token(parser->lexer));
        return NULL;
    }

    s_ast_list *list;
    if (!(list = parse_rule_list(parser)))
        return NULL; /** Could not parse */
    s_ast_input *ast = ast_input_new();
    ast->list = list;

    tok = lex_token(parser->lexer);
    if (!(tok->type == T_NEWLINE
        || tok->type == T_EOF))
        LOG(WARN, "parser: unexpected 'input' token.", NULL);

    token_free(tok);
    return ast;
}
