#include "ft_readline.h"

void	home_toggle(size_t *pos)
{
	while ((*pos))
	{
		write(0, LEFT_ARR, 3);
		(*pos)--;
	}
}

void	end_toggle(size_t *pos, size_t *len)
{
	while ((*pos) < (*len))
	{
		write(0, RIGHT_ARR, 3);
		(*pos)++;
	}
}

void	print_char(char c, hist **current, size_t *pos, size_t *len)
{
	size_t	j;
	hist	*tmp;

	tmp = *current;
	if (*len == tmp->size -1)
	{
		tmp->buffer = realloc(tmp->buffer, tmp->size * 2);
		if (!tmp->buffer)
			return ;
		tmp->size = tmp->size * 2;
	}
	memmove(&tmp->buffer[(*pos) + 1], &tmp->buffer[(*pos)], (*len) - (*pos) + 1);
	tmp->buffer[(*pos)] = c;
	write(0, &tmp->buffer[(*pos)], 1);
	j = (*pos);
	while (j < (*len))
	{
		write(0, "\033[D", 3);
		j++;
	}
	(*len)++;
	(*pos)++;
}

void	which_key(char seq[6], hist **current, size_t *pos, size_t *len)
{
	(void)current;
	if (!strcmp(seq, UP_ARR))	
		;//change_history_up();
	else if (!strcmp(seq, DOWN_ARR))	
		;//change_history_down();
	else if (!strcmp(seq, RIGHT_ARR))	
		;//move_right();
	else if (!strcmp(seq, LEFT_ARR))	
		;//move_left();
	else if (!strcmp(seq, HOME))
		home_toggle(pos);
	else if (!strcmp(seq, END))	
		end_toggle(pos, len);
}

void	verif_seq(char seq[6], hist **current, size_t *pos, size_t *len)
{
	read(0, &seq[1], 1);
	if (seq[1] == '[')
	{
		read(0, &seq[2], 1);
		if (seq[2] == '1')
		{
			read(0, &seq[3], 3);
			;//which_key2(seq[5], current, pos, len);
		}
		else
			which_key(seq, current, pos, len);
	}
}

int		ft_read(hist **current)
{
	size_t	pos = 0;
	size_t	len = 0;
	char	seq[6] = {0};

	while (1)
	{
		int val = read(0, &seq, 1);
		if (val <= 0)
			return -1;
		if (seq[0] == '\n')
			return 0;
		if (seq[0] == 127)
			;//delete_char(current, &pos, &len);
		if (seq[0] == '\033')
			verif_seq(seq, current, &pos, &len);
		if (isPrint(seq[0]))
			print_char(seq[0], current, &pos, &len);
	}
	return 0;
}