#include <unistd.h>
#include <term.h>
#include "wrapper.h"

static int write_char(int c)
{
    write(STDOUT_FILENO, &c, 1);
    return 0;
}

#include <stdio.h>
void my_tputs(const char *str)
{
    tputs(str, 0, write_char);
}
