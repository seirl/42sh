#ifndef INPUT_PRIVATE_H
# define INPUT_PRIVATE_H

# include "input.h"

enum input_type
{
    INPUT_FILE,
    INPUT_STRING,
    INPUT_INTERACTIVE
};
typedef enum input_type e_input_type;

struct input
{
    /** What is this input? */
    e_input_type type;
    /** Where does this input compe from? */
    const char *source;
    /** Get a char from this input. */
    char (*getc)(struct input *input);
    /** Get the next char from this input. */
    char (*topc)(struct input *input);
    /** Internal status of the input*/
    void *_input_state;
};

s_input *input_new(void);
void input_delete(s_input *input);

#endif /* !INPUT_PRIVATE_H */
