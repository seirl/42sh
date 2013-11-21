#include "input_interactive.h"

#include "input_string.h"
#include "readline.h"
#include "string_utils.h"

struct input_interactive
{
    s_string *buf;
};
typedef struct input_interactive s_input_interactive;

char input_interactive_getc(void *input_state)
{
    s_input_interactive *input = input_state;
    char out;
    if (!input->buf)
        input->buf = readline("42sh$ ");
    if ((out = string_getc(input->buf)))
        return out;
    string_free(input->buf);
    input->buf = readline("42sh$ ");
    return string_getc(input->buf);
}

char input_interactive_topc(void *input_state)
{
    s_input_interactive *input = input_state;
    char out;
    if (!input->buf)
        input->buf = readline("42sh$ ");
    if ((out = string_topc(input->buf)))
        return out;
    string_free(input->buf);
    input->buf = readline("42sh$ ");
    return string_topc(input->buf);
}
