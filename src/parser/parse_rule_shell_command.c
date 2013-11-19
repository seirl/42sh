#include "parser_private.h"
#include "parser_macros.h"

static s_ast_shell_cmd *parse_compound_list(s_parser *parser, s_token *tok)
{
    int subshell = 0;

    if (tok->type == T_LPAREN)
        subshell = 1;

    parser_shift_token(parser);

    s_ast_list *list;
    if (!(list = parse_rule_compound_list(parser, 1)))
        RETURN_PARSE_EXPECTED(parser, "List");

    s_ast_shell_cmd *shell_cmd = ast_shell_cmd_new();
    shell_cmd->subshell = subshell;
    shell_cmd->cmd_list = list;

    if (!parse_expect_token(parser, tok->type == T_LPAREN
                                                 ? T_RPAREN : T_RBRACE))
        RETURN_PARSE_EXPECTED(parser, "End of compound list");
    parser_shift_token(parser);

    token_free(tok);
    return shell_cmd;
}

s_ast_shell_cmd *parse_rule_shell_command(s_parser *parser)
{
    s_token *tok;

    tok = lex_look_token(parser->lexer);

    if (tok->type == T_LBRACE || tok->type == T_LPAREN)
        return parse_compound_list(parser, tok);

#define X(Token, Type, Sub, Rule)                         \
    if (tok->type == Token)                               \
    {                                                     \
        parser_shift_token(parser);                       \
        s_ast_shell_cmd *shell_cmd = ast_shell_cmd_new(); \
        shell_cmd->ctrl_structure = Type;                 \
        shell_cmd->ctrl.Sub = Rule(parser);               \
        token_free(tok);                                  \
        return shell_cmd;                                 \
    }
#include "shell_command.def"
#undef X
    token_free(tok);

    return NULL;
}
