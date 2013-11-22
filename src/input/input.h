#ifndef INPUT_H
# define INPUT_H

# include "shell.h"

struct shell;

# define SRC_FILE 4
# define SRC_STR 8

typedef struct input s_input;

s_input *input_create(struct shell *shell, char *src, int mode);
void input_destroy(s_input *input);

#endif /* !INPUT_H */
