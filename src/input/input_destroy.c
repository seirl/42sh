#include "input_private.h"

#include "input_file.h"
#include "input_interactive.h"
#include "input_string.h"

void input_destroy(s_input *input)
{
    switch (input->type)
    {
    case INPUT_FILE:
        input_file_delete(input);
        break;
    case INPUT_STRING:
        input_string_delete(input);
        break;
    case INPUT_INTERACTIVE:
        input_interactive_delete(input);
        break;
    }
    input_delete(input);
}
