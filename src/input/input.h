#ifndef INPUT_H
# define INPUT_H

# include "string_utils.h"

struct input_state
{
    s_string *line;
    size_t index;
};
typedef struct input_state s_input_state;

char input_getc(void *lexer_state);
char input_topc(void *lexer_state);

#endif /* !INPUT_H */
