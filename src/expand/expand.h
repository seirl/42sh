#ifndef EXPAND_H
# define EXPAND_H

# include "shell.h"
# include "string_utils.h"
# include "ast.h"
# include "lexer.h"

s_string *expand_simple_word(s_shell *shell, s_string *word);
s_string *expand_substs_shell(s_shell *shell, s_string *word);
s_string *expand_substs_var(s_shell *shell, s_string *word);
s_string *expand_simple_var(s_shell *shell, s_string *word);
s_string *expand_squote(s_shell *shell, s_string *word);
s_string *expand_dquote(s_shell *shell, s_string *word);
s_string *expand_arithm(s_shell *shell, s_string *word);

s_string *expand_glob(s_shell *shell, s_string *word);

s_string *expand(s_shell *shell, const s_ast_compound_word *input);
s_string *expand_string(s_shell *shell, s_string *str, e_lexer_context lcon);

void remove_special_backslash(s_string *str);

#endif /* !EXPAND_H */
