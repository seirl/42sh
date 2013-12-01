#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 1
#endif

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
#include "shell_eval.h"
#include "expand.h"
#include "eval.h"
#include "shell_private.h"
#include "env.h"
#include "smalloc.h"

#include "env_private.h"

#include <string.h>

static int is_blank(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

static s_string *process_stdout(s_string *ret)
{
    int i;
    for (i = 0; is_blank(ret->buf[i]); ++i)
        continue;
    if (i)
        string_del_nth(ret, 0, i);
    for (i = 0; ret->buf[i]; ++i)
    {
        if (is_blank(ret->buf[i]))
        {
            ret->buf[i] = ' ';
            int j;
            for (j = 1; is_blank(ret->buf[i + j]); ++j)
                continue;
            string_del_nth(ret, i, j - (ret->buf[i + j] == 0 ? 0 : 1));
            if (ret->buf[i] == '\0')
                break;
        }
    }
    if (ret->buf[ret->len] == ' ')
        ret->buf[ret->len] = '\0';
    return ret;
}

static void exec_subshell(s_shell *shell, int pipe_fd[2], char *str)
{
    close(pipe_fd[0]);
    close(1);
    fcntl(pipe_fd[1], F_DUPFD, 1);

    shell_eval_str(shell, str);

    close(pipe_fd[1]);
    exit(0);
}

static s_string *monitor_subshell(pid_t pid, int pipe_fd[2])
{
    s_string *ret = string_create(0);
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
    return process_stdout(ret);
}

s_string *expand_substs_shell(s_shell *shell, s_string *word)
{
    pid_t pid;
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        return NULL;
    pid = fork();
    if (pid == 0)
        exec_subshell(shell, pipe_fd, word->buf);
    return monitor_subshell(pid, pipe_fd);
}
