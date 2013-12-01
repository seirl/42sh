#ifndef EXPAND_H
# define EXPAND_H

# include "shell.h"
# include "string_utils.h"
# include "ast.h"
# include "lexer.h"

/** @brief Expand a token of type WORD  */
s_string *expand_simple_word(s_shell *shell, s_string *word);
/** @brief Expand a token of type SUBST_SHELL */
s_string *expand_substs_shell(s_shell *shell, s_string *word);
/** @brief Expand a var */
s_string *expand_substs_var(s_shell *shell, s_string *word);
/** @brief Expand a var */
s_string *expand_simple_var(s_shell *shell, s_string *word);
/** @brief Expand a string between simple quotes */
s_string *expand_squote(s_shell *shell, s_string *word);
/** @brief Expand a string between double quotes */
s_string *expand_dquote(s_shell *shell, s_string *word);
/** @brief Expand an arithmetic expression */
s_string *expand_arithm(s_shell *shell, s_string *word);
/** @brief Pathname expansion */
s_string *expand_glob(s_shell *shell, s_string *word);
/** @brief Expand a compound word */
s_string *expand(s_shell *shell, const s_ast_compound_word *input);
/** @brief Determine the type of a string and expand it */
s_string *expand_string(s_shell *shell, s_string *str, e_lexer_context lcon);
/** @brief Remove backslash in a dquoted string */
void remove_special_backslash(s_string *str);
/** @brief Split a compound word into multiple nodes */
s_ast_compound_word *split_compound_word(s_shell *shell,
                                         s_ast_compound_word *cw, int type);
/** @brief Split an element into multiples nodes */
void expand_element(s_shell *shell, s_ast_element *elt);
/** @brief Split a wordlist into multiples nodes */
void expand_wordlist(s_shell *shell, s_ast_word_list *elt);
/** @brief Split a list of token */
char **expand_argv(char **argv, int *max_len);

#endif /* !EXPAND_H */
