#include "parser_private.h"

static s_ast_cmd *post_process_cmd(s_ast_cmd *cmd)
{
    if (!cmd->func_dec && !cmd->simple_cmd)
    {
        ast_cmd_delete(cmd);
        return NULL;
    }

    return cmd;
}

s_ast_cmd *parse_rule_command(s_parser *parser)
{
    s_ast_cmd *cmd = ast_cmd_new();

    if ((cmd->shell_cmd = parse_rule_shell_command(parser)))
    {
        cmd->redirections = parse_rule_redirection(parser);
        return cmd;
    }

    s_token *tok = lex_look_token(parser->lexer);
    if (tok->type == T_FUNCTION)
    {
        token_free(tok);
        parser_shift_token(parser);
        cmd->func_dec = parse_rule_funcdec(parser);
        return post_process_cmd(cmd);
    }
    token_free(tok);

    /** We must not process reserved words */
    if (next_word_is_res_word(parser))
        return NULL;

    tok = lex_look_word(parser->lexer);
    if (tok && tok->type == T_FUNCTION_NAME)
        cmd->func_dec = parse_rule_funcdec(parser);
    else
        cmd->simple_cmd = parse_rule_simple_command(parser);
    token_free(tok);

    return post_process_cmd(cmd);
}
