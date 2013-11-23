#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "input_string.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "smalloc.h"

int main(int argc, char **argv)
{
    int ret = 0;

    if (argc < 2)
    {
        fputs("usage: test_parser <INPUT>\n", stderr);
        return 1;
    }

    s_input *input;
    s_lexer *lexer;
    s_parser *parser;
    s_ast_input *ast;

    input = input_string_create(string_create_from(argv[1]), "<INPUT>");
    lexer = lex_create(input);
    parser = parser_create(lexer);
    if ((ast = parse_rule_input(parser)))
    {
        if (!parser_diagnostic(parser))
            ret = 1;
        else if (!parser_eof(parser))
        {
            LOG(ERROR, "Garbage in the lexer after parsing", NULL);
            ret = 1;
        }
        ast_input_delete(ast);
    }
    else
        ret = 1;

    parser_delete(parser);
    lex_delete(lexer);
    input_destroy(input);
    smalloc_clean();

    return ret;
}
