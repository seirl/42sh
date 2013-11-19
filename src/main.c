#include <stdlib.h>
#include "options.h"
#include "shopt.h"
#include "lexer.h"
#include "parser.h"
#include "input.h"
#include "log.h"
#include "ast_printer.h"
#include "smalloc.h"

#include "token.h"

static int parse_input(s_lexer *lexer)
{
    s_parser *parser = parser_create(lexer);
    s_ast_input *ast;
    if ((ast = parse_rule_input(parser)))
    {
        if (shopt_get("ast_print"))
            print_ast(ast, stdout);
        ast_input_delete(ast);
        if (!parser_eof(parser))
        {
            LOG(ERROR, "Garbage in the lexer after parsing", NULL);
            return 1;
        }
    }
    parser_delete(parser);
    return 0;
}

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
        if (lexer == NULL || parse_input(lexer))
            break;
        input_free(lexer, cmd, file);
    } while (repeat);
    smalloc_clean();
    return EXIT_SUCCESS;
}
