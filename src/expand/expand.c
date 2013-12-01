#include "expand.h"
#include "input_string.h"

static s_string *(*const expand_funcs[])(s_shell *, s_string *) =
{
    expand_simple_word,
    expand_simple_var,
    expand_substs_var,
    expand_substs_shell,
    expand_arithm,
    expand_squote,
    expand_dquote,
    expand_substs_shell
};

static s_string *expand_word(s_shell *shell, const s_ast_word *word)
{
    string_rewind(word->str);
    return expand_funcs[word->kind](shell, word->str);
}

s_string *expand(s_shell *shell, const s_ast_compound_word *input)
{
    s_string *r = string_create(0);
    for (const s_ast_compound_word *cw = input; cw; cw = cw->next)
    {
        s_string *expanded_word = expand_word(shell, cw->word);
        string_cat(r, expanded_word);
        string_free(expanded_word);
    }
    return r;
}

static s_string *expand_with_type(s_shell *shell, s_token *tok,
                                  e_lexer_context lcon)
{
    if (lcon == LEX_DQUOTE)
    {
#define X(Ltok, Atok)                                         \
        if (tok->type == Ltok)                                \
            return expand_funcs[Atok](shell, tok->value.str);
#include "dquote_token.def"
#undef X
        return string_duplicate(tok->value.str);
    }
    else
    {
#define X(Ltok, Atok)                                         \
        if (tok->type == Ltok)                                \
            return expand_funcs[Atok](shell, tok->value.str);
#include "token_convert.def"
#undef X
    }
    return NULL;
}

s_string *expand_string(s_shell *shell, s_string *str, e_lexer_context lcon)
{
    s_input *input = input_string_create(str, "dquote");

    s_lexer *lexer = lex_create(shell, input, lcon);
    s_token *tok;
    e_token_type t;
    s_string *ret = string_create(0);
    s_string *sub = NULL;

    do {
        tok = lex_token(lexer);
        t = tok->type;
        sub = expand_with_type(shell, tok, lcon);
        if (t == T_EOF)
            break;
        string_cat(ret, sub);
        if (sub)
            string_free(sub);
        token_free(tok);
    } while (t != T_EOF);
    string_free(sub);
    token_free(tok);
    lex_delete(lexer);
    input_destroy(input);
    return ret;
}
