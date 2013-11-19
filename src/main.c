#include <stdlib.h>
#include "options.h"
#include "lexer.h"
#include "parser.h"
#include "input.h"

#include "token.h"

int main(int argc, char *argv[])
{
    char *file = NULL;
    char *cmd = NULL;
    int repeat = 0;
    int ret = parse_options(argc, argv, &cmd, &file);
    if (ret)
        return ret;
    do {
        s_lexer *lexer = input_to_lexer(cmd, file, &repeat);
        if (lexer == NULL)
            break;
        s_parser *parser = parser_create(lexer);
        (void)parser;
        input_free(lexer, cmd, file);
    } while (repeat);
    return EXIT_SUCCESS;
}
