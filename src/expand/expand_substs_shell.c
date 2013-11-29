#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "string_utils.h"
#include "ast.h"
#include "shell.h"
#include "expand.h"
#include "eval.h"

s_string *expand_substs_shell(s_shell *shell, s_string *word)
{
    (void)shell;
    //TODO: check fd leaks
    s_string *ret = string_create(0);
    pid_t pid;
    int pipe_fd[2];
    pipe(pipe_fd);

    pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[0]);
        close(1);
        fcntl(pipe_fd[1], F_DUPFD, 1);
        char *arg[] = { word->buf, NULL};
        execvp(word->buf, arg);
        close(pipe_fd[1]);
        exit(0);
    }

    close(pipe_fd[1]);

    FILE *input = fdopen(pipe_fd[0], "r");
    int alive = 0;
    while (1)
    {
        int c = fgetc(input);
        if (c >= 0)
            string_putc(ret, c);
        int stat = 0;
        if (alive == 0)
            alive = waitpid(pid, &stat, WNOHANG);
        if (alive > 0 && c == EOF)
            break;
    }
    fclose(input);
    return ret;
}
