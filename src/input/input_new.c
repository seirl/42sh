#include "input_private.h"
#include "smalloc.h"

s_input *input_new(void)
{
    s_input *input = smalloc(sizeof (s_input));

    input->_input_state = NULL;
    input->source = NULL;
    input->topc = NULL;
    input->getc = NULL;
    input->type = 0;

    return input;
}
