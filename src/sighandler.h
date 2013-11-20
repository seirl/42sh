#ifndef SIGHANDLER_H
# define SIGHANDLER_H

# define GDBARGS(Pid)  \
    {                  \
        "gdb",         \
        "-p",          \
        Pid,           \
        "-q",          \
        NULL           \
    }

struct sig_state
{
    int sigquit;
};
typedef struct sig_state s_sig_state;

void signal_init(void);

#endif /* !SIGHANDLER_H */
