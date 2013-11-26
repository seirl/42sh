#include "parser_private.h"

static s_ast_word *word_of_token(s_token *tok)
{
    s_ast_word *word = ast_word_new();

    if (tok->type == T_WORD)
        word->kind = WORD;
#define X(Toktype, Asttype)        \
    else if (tok->type == Toktype) \
        word->kind = Asttype;
#include "wordtoken2ast.def"
#undef X

    word->str = string_duplicate(tok->value.str);

    return word;
}

s_ast_compound_word *parse_compound_word(s_parser *parser)
{
    s_token *tok;
    s_ast_word *word;

    if (!(tok = lex_word(parser->lexer)))
        return NULL;
    word = word_of_token(tok);
    token_free(tok);

    s_ast_compound_word *cw = ast_compound_word_new();

    tok = lex_look_word(parser->lexer);
    if (tok)
    {
        if (tok->concat)
            cw->next = parse_compound_word(parser);
        token_free(tok);
    }

    cw->word = word;

    return cw;
}
