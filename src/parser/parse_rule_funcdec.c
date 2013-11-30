#include "parser_private.h"
#include "parser_macros.h"

s_ast_funcdec *parse_rule_funcdec(s_parser *parser)
{
    s_string *name;
    s_ast_shell_cmd *content;
    s_token *tok;

    if (!(tok = lex_name(parser->lexer)))
        return NULL;
    name = string_duplicate(tok->value.str);
    token_free(tok);

    if (!parse_expect_token(parser, T_LPAREN)
        || !parse_expect_token(parser, T_RPAREN))
    {
        string_free(name);
        RETURN_PARSE_EXPECTED(parser, "()");
    }

    parse_expect_newlines(parser);

    content = parse_rule_shell_command(parser);
    if (!content)
    {
        string_free(name);
        return NULL;
    }

    s_ast_funcdec *fd = ast_func_dec_new();
    fd->name = name;
    fd->shell_cmd = content;

    return fd;
}
