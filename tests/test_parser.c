#include <stdio.h>
#include <stdlib.h>

#include "smalloc.h"
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "log.h"

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
    int ret = 0;

    if (argc < 2)
    {
        fputs("usage: test_parser <INPUT>\n", stderr);
        return 1;
    }

    s_lexer *lexer;
    s_parser *parser;
    s_ast_input *ast;

    str = argv[1];
    lexer = lex_create(dummy_getc, dummy_topc, NULL, "<test>");
    parser = parser_create(lexer);
    if (!(ast = parse_rule_input(parser)) || !parser_diagnostic(parser))
        ret = 1;

    parser_delete(parser);
    smalloc_clean();

    return ret;
}
