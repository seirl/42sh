#include <unistd.h>
#include <term.h>
#include "wrapper.h"

static int write_char(int c)
{
    return write(STDOUT_FILENO, &c, 1) == -1;
}

#include <stdio.h>
void my_tputs(const char *str)
{
    tputs(str, 0, write_char);
}
