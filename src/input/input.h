#ifndef INPUT_H
# define INPUT_H

# include "lexer.h"

s_lexer *input_to_lexer(char *cmd, char *file);
void input_free(s_lexer *lexer, char *cmd, char *file);

#endif /* !INPUT_H */
