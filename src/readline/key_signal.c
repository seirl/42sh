#include <signal.h>
#include <stdio.h>
#include "key_signal.h"

int signal_key(char c)
{
    fflush(stdout);
    if (c == SIGQUIT_VALUE || c == SIGINT_VALUE)
        return 1;
    return 0;
}
