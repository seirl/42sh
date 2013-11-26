#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer_private.h"
#include "smalloc.h"
#include "alias.h"
#include "shopt.h"

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

void expand_alias(s_shell *shell, s_token *tok)
{
    if (tok->type == T_WORD && tok->aliasable == 1 && shell
       && shopt_get(shell, "expand_aliases"))
    {
        s_string *alias = alias_get(shell, tok->value.str);
        if (alias && strcmp(alias->buf, tok->value.str->buf))
        {
            string_free(tok->value.str);
            tok->value.str = string_duplicate(alias);
            expand_alias(shell, tok);
        }
    }
}

