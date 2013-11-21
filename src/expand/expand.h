#ifndef EXPAND_H
# define EXPAND_H

s_string *expand_simple_word(s_shell *shell, s_string *word);
s_string *expand_substs_shell(s_shell *shell, s_string *word);
s_string *expand_substs_var(s_shell *shell, s_string *word);
s_string *expand_simple_var(s_shell *shell, s_string *word);

s_string *expand(s_shell *shell, s_ast_compound_word *input);

#endif /* !EXPAND_H */