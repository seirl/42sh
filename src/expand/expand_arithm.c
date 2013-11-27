#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "env.h"
#include "lexer.h"
#include "token.h"
#include "input_string.h"
#include "evalexpr.h"

s_string *expand_arithm(s_shell *shell, s_string *word)
{
    s_string *r;
    r = expand_string(shell, string_duplicate(word), LEX_DQUOTE);
    long result = evalexpr(r->buf);
    string_free(r);

    return string_itoa(result);
}
