#include "input_private.h"
#include "lexer_private.h"
#include "location.h"

char lex_getc(s_lexer *lexer)
{
    char c = lexer->input->getc(lexer->input);

    return c;
}

char lex_topc(s_lexer *lexer)
{
    return lexer->input->topc(lexer->input);
}
