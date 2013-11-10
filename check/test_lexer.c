#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

char *str;
size_t pos = 0;

char dummy_getc(void *state)
{
    (void)state;
    return str[pos++];
}

char dummy_topc(void *state)
{
    (void)state;
    return str[pos];
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fputs("usage: test_lexer <INPUT>\n", stderr);
        return 1;
    }

    s_token *tok;
    s_lexer *lexer;

    str = argv[1];
    lexer = lex_create(dummy_getc, dummy_topc, "<test>");
    do {
        tok = lex_token(lexer);
        token_print(tok);
    } while (tok->type != T_EOF);

    lex_delete(lexer);
}
