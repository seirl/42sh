#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios *g_initial_termios = NULL;

void setup_terminal(void) // TODO: test
{
    g_initial_termios = malloc(sizeof (struct termios));
    tcgetattr(STDIN_FILENO, g_initial_termios);

    struct termios new_termios = *g_initial_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSADRAIN, &new_termios);
}

void reset_terminal(void) // TODO: test
{
    tcsetattr(STDIN_FILENO, TCSADRAIN, g_initial_termios);
    g_initial_termios = NULL;
}
