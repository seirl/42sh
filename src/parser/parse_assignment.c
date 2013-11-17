#include "parser_private.h"
#include "smalloc.h"

s_ast_assignment *assignment_new(void)
{
    s_ast_assignment *assignment = smalloc(sizeof (s_ast_assignment));

    assignment->name = NULL;
    assignment->value = NULL;

    return assignment;
}

s_ast_assignment *parse_assignment(s_parser *parser)
{
    s_token *tok;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_ASSIGNMENT_WORD)
    {
        s_ast_assignment *assignment = assignment_new();
        assignment->name = string_duplicate(tok->value.str);
        token_free(tok);
        parser_shift_token(parser);

        tok = lex_look_word(parser->lexer);
        if (tok && tok->concat)
            assignment->value = parse_compound_word(parser);

    }
    token_free(tok);

    return NULL;
}
