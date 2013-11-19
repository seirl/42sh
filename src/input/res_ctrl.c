#include "shopt.h"
#include "res_ctrl.h"
#include "lexer.h"
#include "parser.h"
#include "log.h"
#include "smalloc.h"
#include "exec.h"
#include "token.h"
#include "input_file.h"

static int load_rc(const char *file)
{
    FILE *f;
    s_lexer *lexer;
    s_parser *parser;
    s_ast_input *ast;
    f = fopen(file, "r");
    if (f)
    {
        lexer = lex_create(input_file_getc, input_file_topc, f, "rc");
        parser = parser_create(lexer);
        if ((ast = parse_rule_input(parser)))
        {
            if (!parser_eof(parser))
            {
                LOG(ERROR, "Garbage in the lexer after parsing", NULL);
                return 1;
            }
            exec_ast_input(ast);
            vars_free();
            funcs_free();
        }
        parser_delete(parser);
        fclose(f);
    }
    return 0;
}

int rc_file_load(void)
{
    load_rc("/etc/42shrc");
    char *home = getenv("HOME");
    if (home)
    {
        char path[255];
        sprintf(path, "%s/.42shrc", home);
        load_rc(path);
    }
    return 1;
}
