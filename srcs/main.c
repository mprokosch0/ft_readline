#include "ft_readline.h"

static void	reset_and_go(struct termios *old)
{
	reset_termios(old);
	write(0, "\n", 1);
}


char	*ft_readline(char *display)
{
	struct termios	new1, old;
	hist			history = {.entries = {NULL}, .entries_sizes = {0}, .curr_entry = NULL, .curr_entry_size = 0,
								.pos = 0, .pos_in_file = 0, .size = 0, .fd = 0, .on_curr = true};
	char			*res = NULL;

	load_history(&history);
	if (!history.curr_entry)
	{
		free_history(&history);
		return NULL;
	}
	if (isatty(STDIN_FILENO))
		write(0, display, strlen(display));
	init_termios(&new1, &old);
	int val = ft_read(&history);
	if (val == -1)
		return NULL;
	reset_and_go(&old);
	res = strdup((history.on_curr) ? history.curr_entry : history.entries[history.pos]);
	free_history(&history);
	return (res);
}

int main(void)
{
	char *str = NULL;
	int a = 10;
	while (a--)
	{
		str = ft_readline("coucou$> ");
		if (str[0])
			printf("%s\n", str);
		free(str);
	}
	return 0;
}