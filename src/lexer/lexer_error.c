#include "lexer_private.h"
#include "log.h"

e_lexer_error lex_error(s_lexer *lexer)
{
    return lexer->error;
}

int lex_error_set(s_lexer *lexer, e_lexer_error error)
{
    if (error == E_LEX_UNBALANCED)
        LOG(WARN, "Unbalanced input", NULL);
    lexer->error = error;
    return error;
}
