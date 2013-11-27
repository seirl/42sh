#ifndef EXPAND_WORDLIST_H
# define EXPAND__WORDLIST_H

# include "shell.h"
# include "string_utils.h"
# include "ast.h"
# include "lexer.h"

void expand_wordlist(s_shell *shell, s_ast_word_list *elt);

#endif /* !EXPAND_WORDLIST_H */
