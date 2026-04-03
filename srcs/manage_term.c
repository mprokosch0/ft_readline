#include "ft_readline.h"

void	init_termios(struct termios *new1, struct termios *old)
{
	memset(old, 0, sizeof(struct termios));
	tcgetattr(0, old);
	*new1 = *old;
	new1->c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, new1);
}

void	reset_termios(struct termios *old)
{
	tcsetattr(0, TCSANOW, old);
}
