#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "exec.h"
#include "input_string.h"
#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "smalloc.h"

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

    s_shell *shell = shell_new();
    s_input *input;
    s_lexer *lexer;
    s_parser *parser;
    s_ast_input *ast;

    input = input_string_create(string_create_from(argv[1]), "<INPUT>");
    lexer = lex_create(shell, input, 1);
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
        else
            exec_ast_input(shell, ast);
        ast_input_delete(ast);
    }
    else
        ret = 1;

    parser_delete(parser);
    lex_delete(lexer);
    input_destroy(input);
    shell_delete(shell);
    smalloc_clean();
    return ret;
}
