#include "parser_private.h"
#include "parser_macros.h"
#include "ast.h"

static s_ast_word_list *parse_matches(s_parser *parser)
{
    s_ast_compound_word *word;
    if (!(word = parse_compound_word(parser)))
        RETURN_PARSE_EXPECTED(parser, "case match");

    s_ast_word_list *words = ast_word_list_new();
    words->word = word;

    if (lex_look_token_type(parser->lexer) == T_PIPE)
    {
        parser_shift_token(parser);
        words->next = parse_matches(parser);
    }

    return words;
}

s_ast_case_item *parse_rule_case_item(s_parser *parser)
{
    e_token_type tok_type = lex_look_token_type(parser->lexer);
    if (tok_type == T_LPAREN)
        parser_shift_token(parser);

    s_ast_case_item *item = ast_case_item_new();
    if (!(item->match = parse_matches(parser)))
    {
        ast_case_item_delete(item);
        RETURN_PARSE_EXPECTED(parser, "case match");
    }

    if (!parse_expect_token(parser, T_RPAREN))
    {
        ast_case_item_delete(item);
        RETURN_PARSE_EXPECTED(parser, ")");
    }

    parse_expect_newlines(parser);

    item->cmd_list = parse_rule_compound_list(parser, 0);

    return item;
}
