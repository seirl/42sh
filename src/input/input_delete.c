#include "input_private.h"
#include "smalloc.h"

void input_delete(s_input *input)
{
    sfree(input);
}
