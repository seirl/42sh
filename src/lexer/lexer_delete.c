#include "lexer_private.h"
#include "smalloc.h"

void lex_delete(s_lexer *lexer)
{
    string_free(lexer->working_buffer);
    sfree(lexer);
}
