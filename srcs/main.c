#include "ft_readline.h"

static void	reset_and_go(struct termios *old)
{
	reset_termios(old);
	write(0, "\n", 1);
}

void	free_all(void)
{
	static_infos(NULL, 0);
}

void	adjust_history(hist **head, hist **tail, hist *current)
{
	delete_node(head, tail, *tail);
	move_to_last(head, tail, current);
}

char	*ft_readline(char *display)
{
	struct termios	new1, old;
	static bool		init = false;
	static hist		*history_head = NULL;
	static hist		*history_tail = NULL;
	char 			*res = NULL;
	hist			*current;

	if (!init)
	{
		atexit(free_all);
		static_infos(&history_head, 1);
	}
	if (!hist_add_back(&history_head, &history_tail, new_node()))
	{
		write(0, "\n", 1);
		return res;
	}
	current = history_tail;
	if (isatty(STDIN_FILENO))
		write(0, display, strlen(display));
	init_termios(&new1, &old);
	int val = ft_read(&current);
	if (val == -1)
		return NULL;
	reset_and_go(&old);
	if (current != history_tail)
		adjust_history(&history_head, &history_tail, current);
	else if (history_tail->prev && !strcmp(history_tail->buffer, history_tail->prev->buffer))
	{
		delete_node(&history_head, &history_tail, history_tail);
		current = history_tail;
	}
	if (!strlen(current->buffer))
	{
		delete_node(&history_head, &history_tail, history_tail);
		return strdup("");
	}
	return (strdup(current->buffer));
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