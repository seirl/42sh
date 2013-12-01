#include "lexer_private.h"
#include "alias.h"
#include "input_string.h"
#include "shopt.h"
#include "hashtbl.h"

int update_alias_delimiter(s_lexer *lexer)
{
    int old = lexer->prev_delim;
    lexer->prev_delim = (lexer->token_type == T_NEWLINE
                        || lexer->token_type == T_AND
                        || lexer->token_type == T_PIPE
                        || lexer->token_type == T_AND_IF
                        || lexer->token_type == T_OR_IF
                        || lexer->token_type == T_EOF
                        || lexer->token_type == T_SEMI);
    return old;
}

static int expand_alias_rec(s_shell *shell, s_token *tok, s_hashtbl *h)
{
    s_string *alias = alias_get(shell, tok->value.str);
    if (alias)
    {
        // Loop detected
        if (hashtbl_get(h, alias))
            return -1;

        hashtbl_set(h, alias, alias);
        string_free(tok->value.str);
        tok->value.str = string_duplicate(alias);
        int ret = expand_alias_rec(shell, tok, h);
        return ret == 0 ? 1 : ret;
    }
    return 0;
}

s_token *expand_alias(s_shell *shell, s_lexer *lexer, s_token *tok)
{
    int aliased = 0;
    if (tok->type == T_WORD && tok->aliasable == 1 && shell
        && shopt_get(shell, "expand_aliases"))
    {
        s_hashtbl *h = hashtbl_init(hash_string, cmp_string, NULL, NULL);
        aliased = expand_alias_rec(shell, tok, h);
        hashtbl_free(h);
    }
    if (aliased != 1)
        return tok;

    s_string *input_str = string_duplicate(tok->value.str);
    s_input *input = input_string_create(input_str, "alias");
    lexer->sublexer = lex_create(shell, input, LEX_ALL);
    token_free(tok);
    return lex_token(lexer->sublexer);
}
