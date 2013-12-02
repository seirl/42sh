#include "parser_private.h"
#include "parser_macros.h"
#include "ast.h"
#include "lexer.h"

static s_ast_compound_word *parse_rule_braces_rec(s_parser *parser)
{
    s_token *tok;
    s_ast_compound_word *cw = ast_compound_word_new();

    if (lex_look_token_type(parser->lexer) == T_RBRACE)
    {
        tok = lex_token(parser->lexer);
        cw->word = word_of_token(tok);
        token_free(tok);
    }
    else
    {
        tok = lex_look_word(parser->lexer);
        if (!tok)
        {
            token_free(tok);
            RETURN_PARSE_UNEXPECTED(parser, lex_look_token(parser->lexer));
        }
        parser_shift_word(parser);
        cw->word = word_of_token(tok);
        token_free(tok);
        cw->next = parse_rule_braces_rec(parser);
    }

    return cw;
}

s_ast_compound_word *parse_rule_braces(s_parser *parser)
{
    if (lex_look_token_type(parser->lexer) != T_LBRACE)
        return NULL;

    // Get {
    s_token *tok = lex_token(parser->lexer);
    s_ast_compound_word *cw = ast_compound_word_new();
    cw->word = word_of_token(tok);
    token_free(tok);

    cw->next = parse_rule_braces_rec(parser);
    return cw;
}
