#include "parser_private.h"
#include "parser_macros.h"

static s_ast_word_list *parse_word_list(s_parser *parser)
{
    s_ast_compound_word *cw;

    if (!(cw = parse_compound_word(parser)))
        return NULL;

    s_ast_word_list *wl = ast_word_list_new();

    wl->word = cw;
    wl->next = parse_word_list(parser);

    return wl;
}

static s_ast_word_list *parse_for_in(s_parser *parser)
{
    s_token *tok;
    s_ast_word_list *values;

    parser_shift_token(parser); /** shift 'in' */
    values = parse_word_list(parser);
    tok = lex_token(parser->lexer);
    if (!(tok->type == T_NEWLINE || tok->type == T_SEMI))
        RETURN_PARSE_EXPECTED(parser, "; or newline");
    token_free(tok);

    parse_expect_newlines(parser);

    return values;
}

s_ast_for *parse_rule_for(s_parser *parser)
{
    s_token *tok;
    s_ast_word *identifier;
    s_ast_word_list *values = NULL;
    s_ast_list *list;

    /** FIXME: should be lex_name */
    if (!(tok = lex_word(parser->lexer)))
        RETURN_PARSE_EXPECTED(parser, "for `name`");

    identifier = word_of_token(tok);
    token_free(tok);

    parse_expect_newlines(parser);
    tok = lex_look_token(parser->lexer);
    if (tok->type == T_IN)
        values = parse_for_in(parser);
    token_free(tok);

    if (!(list = parse_rule_do_group(parser)))
        RETURN_PARSE_EXPECTED(parser, "do group");

    s_ast_for *for_n = ast_for_new();
    for_n->identifier = identifier;
    for_n->cmd_list = list;
    for_n->values = values;
    return for_n;
}
