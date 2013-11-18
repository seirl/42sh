#include "parser_private.h"

s_ast_compound_word *parse_compound_word(s_parser *parser)
{
    s_token *tok;
    s_ast_word *word;

    if (!(tok = lex_look_word(parser->lexer)))
        return NULL;

    word = word_of_token(tok);
    token_free(tok);
    parser_shift_word(parser);

    s_ast_compound_word *cw = ast_compound_word_new();

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
