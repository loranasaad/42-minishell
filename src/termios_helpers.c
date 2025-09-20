#include <termios.h>
#include <unistd.h>

static struct termios g_orig;
static int g_saved = 0;

int	termios_disable_echoctl(void) 
{
	struct termios t;
	if (tcgetattr(STDIN_FILENO, &g_orig) == -1) return -1;
	g_saved = 1;
	t = g_orig;
	t.c_lflag &= ~(ECHOCTL);              //hides ctrl c and the other one
    return tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	termios_restore(void)
{
    if (g_saved) tcsetattr(STDIN_FILENO, TCSANOW, &g_orig);
}