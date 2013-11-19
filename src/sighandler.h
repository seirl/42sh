#ifndef SIGHANDLER_H
# define SIGHANDLER_H

struct sig_state
{
    int sigquit;
};
typedef struct sig_state s_sig_state;

void signal_init(void);

#endif /* !SIGHANDLER_H */
