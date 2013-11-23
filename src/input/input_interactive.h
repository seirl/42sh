#ifndef INPUT_INTERACTIVE_H
# define INPUT_INTERACTIVE_H

# include "input_private.h"
# include "shell.h"

s_input *input_interactive_create(s_shell *shell);
void input_interactive_delete(s_input *input);

char input_interactive_getc(s_input *input);
char input_interactive_topc(s_input *input);
int input_interactive_next(s_input *input);

#endif /* !INPUT_INTERACTIVE_H */
