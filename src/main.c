#include <stdlib.h>
#include "options.h"
#include "lexer.h"
#include "parser.h"
#include "input.h"

int main(int argc, char *argv[])
{
    char *cmd = NULL;
    int ret = parse_options(argc, argv, &cmd);
    if (ret)
        return ret;
    s_lexer *lexer = input_to_lexer(cmd);
    s_parser *parser = parser_create(lexer);
    (void)parser;
    return EXIT_SUCCESS;
}
