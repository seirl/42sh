#include "input_interactive.h"

#include "input_string.h"
#include "readline.h"
#include "shell_private.h"
#include "smalloc.h"
#include "string_utils.h"

struct input_state_interactive
{
    s_string *buf;
    s_shell *shell;
};
typedef struct input_state_interactive s_input_state_interactive;

s_input *input_interactive_create(s_shell *shell)
{
    s_input *input = input_new();

    input->type = INPUT_INTERACTIVE;
    input->getc = input_interactive_getc;
    input->topc = input_interactive_topc;
    input->next = input_interactive_next;
    s_input_state_interactive *state = smalloc(sizeof
                                               (s_input_state_interactive));
    state->shell = shell;
    state->buf = NULL;
    input->_input_state = state;
    input->source = "<interactive>";

    return input;
}

void input_interactive_delete(s_input *input)
{
    s_input_state_interactive *state = input->_input_state;
    string_free(state->buf);
    sfree(state);
}

char input_interactive_getc(s_input *input)
{
    s_input_state_interactive *state = input->_input_state;
    return state->buf ? string_getc(state->buf) : '\0';
}

char input_interactive_topc(s_input *input)
{
    s_input_state_interactive *state = input->_input_state;
    return state->buf ? string_topc(state->buf) : '\0';
}

int input_interactive_next(s_input *input)
{
    s_input_state_interactive *state = input->_input_state;
    string_free(state->buf);
    state->buf = readline(state->shell, "42sh$ ");
    return !!state->buf;
}
