#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer_private.h"
#include "smalloc.h"
#include "alias.h"
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

static void expand_alias_rec(s_shell *shell, s_token *tok, s_hashtbl *h)
{
    s_string *alias = alias_get(shell, tok->value.str);
    if (alias)
    {
        if (hashtbl_get(h, alias) == NULL)
        {
            string_free(tok->value.str);
            tok->value.str = string_duplicate(alias);
            hashtbl_set(h, alias, alias);
            expand_alias_rec(shell, tok, h);
        }
    }
}

void expand_alias(s_shell *shell, s_token *tok)
{
    if (tok->type == T_WORD && tok->aliasable == 1 && shell
        && shopt_get(shell, "expand_aliases"))
    {
        s_hashtbl *h = hashtbl_init(hash_string, cmp_string, NULL, NULL);
        s_string *word_copy = string_duplicate(tok->value.str);
        hashtbl_set(h, word_copy, word_copy);
        expand_alias_rec(shell, tok, h);
        string_free(word_copy);
        hashtbl_free(h);
    }
}

