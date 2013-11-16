#include "parser_private.h"
#include "smalloc.h"

s_ast_compound_word *cw_new(void)
{
    s_ast_compound_word *cw = smalloc(sizeof (s_ast_compound_word));

    cw->word = NULL;
    cw->next = NULL;

    return cw;
}

s_ast_compound_word *parse_compound_word(s_parser *parser)
{
    s_token *tok;
    s_ast_word *word;

    tok = lex_look_word(parser->lexer);
    if (tok)
    {
        word = word_of_token(tok);
        parser_shift_word(parser);
        token_free(tok);
    }
    else
        return NULL;

    s_ast_compound_word *cw = cw_new();

    tok = lex_look_word(parser->lexer);
    if (tok)
    {
        if (tok->concat)
            // XXX: Check for token type?
            cw->next = parse_compound_word(parser);
        token_free(tok);
    }

    cw->word = word;

    return cw;
}
