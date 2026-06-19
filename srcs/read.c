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

void	move_left(size_t *pos)
{
	if (*pos)
	{
		write(0, LEFT_ARR, 3);
		(*pos)--;
	}
}

void	move_right(size_t *pos, size_t *len)
{
	if (*pos < *len)
	{
		write(0, RIGHT_ARR, 3);
		(*pos)++;
	}
}

void	change_history(hist *history, size_t *pos, size_t *len)
{
	while (*pos < *len)
	{
		write(0, RIGHT_ARR, 3);
		(*pos)++;
	}
	while (*pos)
	{
		write(0, "\b \b", 3);
		(*pos)--;
	}
	char *buffer = (history->on_curr) ? history->curr_entry : history->entries[history->pos];
	size_t newLen = strlen(buffer);
	write(0, buffer, newLen);
	*len = newLen;
	*pos = *len;
}

bool	which_key(char seq[6], hist *history, size_t *pos, size_t *len)
{
	if (!strcmp(seq, UP_ARR))
	{
		if (history->pos_in_file == 0)
			return true;
		if (history->on_curr)
			history->on_curr = false;
		else
		{
			if (!history->pos && !load_prev_part_history(history))
				return false;
			history->pos--;
			history->pos_in_file--;
		}
		change_history(history, pos, len);
	}
	else if (!strcmp(seq, DOWN_ARR))
	{
		if (!history->on_curr && (history->pos_in_file >= history->size - 1 || !history->entries[history->pos + 1]))
			history->on_curr = true;
		if (!history->on_curr)
		{
			if (history->pos + 1 >= HIST_SIZE && !load_next_part_history(history))
				return false;
			history->pos++;
			history->pos_in_file++;
		}
		change_history(history, pos, len);
	}
	else if (!strcmp(seq, RIGHT_ARR))	
		move_right(pos, len);
	else if (!strcmp(seq, LEFT_ARR))	
		move_left(pos);
	else if (!strcmp(seq, HOME))
		home_toggle(pos);
	else if (!strcmp(seq, END))	
		end_toggle(pos, len);
	return true;
}

void	delete_char(char **buffer, size_t *pos, size_t *len)
{
	size_t	j;

	if ((*pos) > 0)
	{
		memmove(&(*buffer)[(*pos) - 1], &(*buffer)[(*pos)], (*len) - (*pos) + 1);
		(*len)--;
		(*pos)--;
		write(0, "\033[D \033[D", 7);
		write(0, &(*buffer)[(*pos)], strlen(&(*buffer)[(*pos)]));
		write(0, " \033[D", 4);
		j = (*pos);
		while (j < (*len))
		{
			write(0, "\033[D", 3);
			j++;
		}
	}
}

bool	print_char(char c, char **buffer, size_t *size, size_t *pos, size_t *len)
{
	size_t	j;

	if (*len == *size -1)
	{
		char *tmp = realloc(*buffer, *size * 2);
		if (!tmp)
			return false;
		*buffer = tmp;
		*size = *size * 2;
	}
	memmove(&(*buffer)[(*pos) + 1], &(*buffer)[(*pos)], (*len) - (*pos) + 1);
	(*buffer)[(*pos)] = c;
	write(0, &(*buffer)[(*pos)], strlen(&(*buffer)[(*pos)]));
	j = (*pos);
	while (j < (*len))
	{
		write(0, "\033[D", 3);
		j++;
	}
	(*len)++;
	(*pos)++;
	return true;
}

bool	verif_seq(char seq[6], hist *history, size_t *pos, size_t *len)
{
	if (read(0, &seq[1], 1) < 0)
		return false;
	if (seq[1] == '[')
	{
		if (read(0, &seq[2], 1) < 0)
			return false;
		if (seq[2] == '1')
		{
			if (read(0, &seq[3], 3) < 0)
				return false;
			;//which_key2(seq[5], current, pos, len);
		}
		else if (!which_key(seq, history, pos, len))
			return false;
			
	}
	return true;
}

int		ft_read(hist *history)
{
	size_t	pos = 0;
	size_t	len = 0;
	char	seq[6] = {0};

	while (1)
	{
		char	**buffer = (history->on_curr) ? &history->curr_entry : &history->entries[history->pos];
		size_t	*size = (history->on_curr) ? &history->curr_entry_size : &history->entries_sizes[history->pos];
		int val = read(0, &seq, 1);
		if (val <= 0)
			return -1;
		if (seq[0] == '\n')
			return 0;
		if (seq[0] == 127)
			delete_char(buffer, &pos, &len);
		if (seq[0] == '\033')
			if (!verif_seq(seq, history, &pos, &len))
				return -1;
		if (isPrint(seq[0]))
			if (!print_char(seq[0], buffer, size, &pos, &len))
				return -1;
	}
	return 0;
}