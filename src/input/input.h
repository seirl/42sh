#ifndef INPUT_H
# define INPUT_H

struct shell;

typedef struct input s_input;

s_input *input_create(struct shell *shell, char **cmd, char *file, int *repeat);
void input_destroy(s_input *input);

#endif /* !INPUT_H */
