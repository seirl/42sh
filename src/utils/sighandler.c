#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "sighandler.h"
#include "macros.h"

#ifdef CATCH_SEGFAULT
static void segfault_callback()
{
    pid_t pid_parent = getpid();
    pid_t pid_gdb = fork();
    if (pid_gdb < 0)
        exit(139);
    else if (pid_gdb == 0)
    {
        int stdin_pipe[2];
        int stdout_pipe[2];
        stdin_pipe[0] = 0;
        stdin_pipe[1] = 0;
        stdout_pipe[0] = 0;
        stdout_pipe[1] = 0;
        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        char pid_s[16];
        sprintf(pid_s, "%d", pid_parent);
        char *gdb_args[] = GDBARGS(pid_s);
        execvp(gdb_args[0], gdb_args);
        exit(139);
    }
    int status = 0;
    waitpid(pid_gdb, &status, 0);
    exit(139);
}
#endif

void signal_init(void)
{
#ifdef CATCH_SEGFAULT
    struct sigaction action;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_handler = segfault_callback;
    sigaction(SIGSEGV, &action, NULL);
#endif
}
