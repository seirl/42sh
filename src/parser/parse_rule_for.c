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
    s_ast_word_list *values;

    parser_shift_token(parser); /** shift 'in' */

    values = parse_word_list(parser);

    if (!parse_expect_newline_or_semi(parser))
    {
        ast_word_list_delete(values);
        RETURN_PARSE_EXPECTED(parser, "; or newline");
    }

    parse_expect_newlines(parser);

    return values;
}

s_ast_for *parse_rule_for(s_parser *parser)
{
    s_token *tok;
    s_ast_word_list *values = NULL;
    s_ast_list *list;

    if (!(tok = lex_name(parser->lexer)))
        RETURN_PARSE_EXPECTED(parser, "for `name`");

    s_ast_word *identifier = ast_word_new();
    identifier->kind = WORD;
    identifier->str = string_duplicate(tok->value.str);
    token_free(tok);

    parse_expect_newlines(parser);

    e_token_type tok_type = lex_look_token_type(parser->lexer);
    if (tok_type == T_IN)
        values = parse_for_in(parser);

    if (!(list = parse_rule_do_group(parser)))
    {
        ast_word_list_delete(values);
        ast_word_delete(identifier);
        RETURN_PARSE_EXPECTED(parser, "do group");
    }

    s_ast_for *for_n = ast_for_new();
    for_n->identifier = identifier;
    for_n->cmd_list = list;
    for_n->values = values;
    return for_n;
}
