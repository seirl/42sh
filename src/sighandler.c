#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include "sighandler.h"

static char *gdb_args[] =
{
    "gdb",
    "-p",
    NULL,
    "-q",
    NULL
};

static void segfault_callback()
{
    fprintf(stderr, "[SEGFAULT] attaching gdb... have fun :)\n");
    pid_t pid_parent = getpid();
    pid_t pid_gdb = fork();
    if (pid_gdb < 0)
    {
        fprintf(stderr, "Failed to fork for GDB\n");
        return;
    }
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
        gdb_args[2] = pid_s;
        execvp(gdb_args[0], gdb_args);
        exit(1);
    }
    //int status = 0;
    //waitpid(pid_gdb, &status, 0);
}

void signal_init()
{
    struct sigaction action;
    action.sa_handler = segfault_callback;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGSEGV, &action, NULL);
}
