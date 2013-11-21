#ifndef EXPAND_H
# define EXPAND_H

# include "shell.h"
# include "string_utils.h"
# include "ast.h"

s_string *expand_simple_word(const s_shell *shell, const s_string *word);
s_string *expand_substs_shell(const s_shell *shell, const s_string *word);
s_string *expand_substs_var(const s_shell *shell, const s_string *word);
s_string *expand_simple_var(const s_shell *shell, const s_string *word);

s_string *expand(const s_shell *shell, const s_ast_compound_word *input);

#endif /* !EXPAND_H */
