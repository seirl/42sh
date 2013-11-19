#include "parser_private.h"

s_ast_assignment *parse_assignment(s_parser *parser)
{
    s_ast_assignment *assignment = NULL;
    s_token *tok;

    tok = lex_look_token(parser->lexer);
    if (tok->type == T_ASSIGNMENT_WORD)
    {
        assignment = ast_assignment_new();
        assignment->name = string_duplicate(tok->value.str);
        token_free(tok);
        parser_shift_token(parser);

        parser_shift_token(parser); /** Shift '=' */

        tok = lex_look_word(parser->lexer);
        if (tok && tok->concat)
            assignment->value = parse_compound_word(parser);

    }
    token_free(tok);

    return assignment;
}
