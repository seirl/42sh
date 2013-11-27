#include "parser_private.h"
#include "parser_macros.h"
#include "ast.h"

s_ast_case *parse_rule_case(s_parser *parser)
{
    s_ast_compound_word *name = parse_compound_word(parser);

    parse_expect_newlines(parser);
    if (!parse_expect_token(parser, T_IN))
    {
        ast_compound_word_delete(name);
        RETURN_PARSE_EXPECTED(parser, "'in'");
    }

    parse_expect_newlines(parser);

    s_ast_case *mycase = ast_case_new();
    mycase->word = name;;
    mycase->clauses = parse_rule_case_clause(parser);

    if (!parse_expect_token(parser, T_ESAC))
    {
        ast_case_delete(mycase);
        RETURN_PARSE_EXPECTED(parser, "'esac'");
    }

    return mycase;
}
