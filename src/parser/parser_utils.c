#include "parser_private.h"
#include "token.h"
#include "log.h"

s_ast_word *word_of_token(s_token *tok)
{
    s_ast_word *word = ast_word_new();

    if (tok->type == T_WORD)
        word->kind = WORD;
    else if (tok->type == T_NAME)
        word->kind = EXPAND_VAR;
    else
        LOG(ERROR, "Bad word token type", NULL);

    word->str = string_duplicate(tok->value.str);

    return word;
}

int parser_eof(s_parser *parser)
{
    s_token *tok;
    tok = lex_look_token(parser->lexer);
    if (tok->type != T_EOF)
    {
        token_free(tok);
        return 0;
    }
    token_free(tok);
    return 1;
}

void parser_shift_token(s_parser *parser)
{
    token_free(lex_token(parser->lexer));
}

void parser_shift_word(s_parser *parser)
{
    token_free(lex_word(parser->lexer));
}

int next_word_is_res_word(s_parser *parser)
{
    s_token *tok = lex_look_token(parser->lexer);

    if (token_is_res_word(tok))
    {
        token_free(tok);
        return 1;
    }
    token_free(tok);

    tok = lex_look_word(parser->lexer);
    if (!tok)
        return 0; /** Not a word, so not a res_word, maybe a redirection. */
    token_free(tok);

    return 0;
}
