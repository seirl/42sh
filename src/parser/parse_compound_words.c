#include "parser_private.h"

static s_ast_compound_word *concat_braces(s_parser *parser,
                                          s_ast_compound_word *cw)
{
    if (!(cw->next = parse_rule_braces(parser)))
        return cw;
    s_ast_compound_word *head = cw;

    while (cw->next)
        cw = cw->next;
    cw->next = parse_compound_word(parser);

    return head;
}

static s_ast_compound_word *parse_compound_word_tail(s_parser *parser,
                                                     s_ast_compound_word *cw)
{
    s_token *tok = lex_look_word(parser->lexer);
    if (tok && tok->concat)
    {
        cw->next = parse_compound_word(parser);
        token_free(tok);
        return cw;
    }
    token_free(tok);
    tok = lex_look_token(parser->lexer);
    if (tok->concat)
        concat_braces(parser, cw);
    token_free(tok);
    return cw;
}

s_ast_compound_word *parse_compound_word(s_parser *parser)
{
    s_token *tok;
    s_ast_word *word;
    s_ast_compound_word *cw;

    if (!(cw = parse_rule_braces(parser)))
    {
        if (!(tok = lex_word(parser->lexer)))
            return NULL;
        word = word_of_token(tok);
        token_free(tok);

        cw = ast_compound_word_new();
        cw->word = word;
    }

    return parse_compound_word_tail(parser, cw);
}
