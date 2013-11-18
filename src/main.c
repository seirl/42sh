#include <stdlib.h>
#include "options.h"
#include "lexer.h"
#include "parser.h"
#include "input.h"

int main(int argc, char *argv[])
{
    int ret = parse_options(argc, argv);
    if (ret)
        return ret;

    s_lexer *lexer = lex_create(input_getc, input_topc, "<stdin>");
    s_parser *parser = parser_create(lexer);
    (void)parser;
    return EXIT_SUCCESS;
}
