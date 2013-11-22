#ifndef INPUT_H
# define INPUT_H

# include "shell.h"

struct shell;

typedef struct input s_input;

s_input *input_create(struct shell *shell,
                      char **cmd,
                      char *file);
void input_destroy(s_input *input);

#endif /* !INPUT_H */
