#include "ft_readline.h"

bool	load_history(hist *history)
{
	history->fd = open(".ft_readline_history", O_CREAT | O_RDWR, 0644);
	if (history->fd < 0)
		return false;
	char *line = NULL;
	off_t end = lseek(history->fd, 0, SEEK_END);
	lseek(history->fd, 0, SEEK_SET);
	while ((line = get_next_line(history->fd)) != NULL)
	{
		if (history->pos >= HIST_SIZE)
		{
			if (history->entries[0])
				free(history->entries[0]);
			memmove(&history->entries[0], &history->entries[1], (HIST_SIZE - 1) * sizeof(char *));
			memmove(&history->entries_sizes[0], &history->entries_sizes[1], (HIST_SIZE - 1) * sizeof(size_t));
			history->pos--;
		}
		history->entries[history->pos] = line;
		history->entries_sizes[history->pos] = strlen(line);
		if (history->entries[history->pos][history->entries_sizes[history->pos] - 1] == '\n')
			history->entries[history->pos][history->entries_sizes[history->pos] - 1] = '\0';
		history->pos++;
		history->size++;
	}
	off_t stop = lseek(history->fd, 0, SEEK_CUR);
	if (stop != end)
		return false;
	if (history->pos > 0)
		history->pos--;
	history->pos_in_file = history->size - 1;
	history->curr_entry = calloc(4096, sizeof(char));
	history->curr_entry_size = 4096;
	return true;
}

bool	load_prev_part_history(hist *history)
{
	lseek(history->fd, 0, SEEK_SET);
	int diff = history->pos_in_file - HIST_SIZE;
	if (diff < 0)
		diff = 0;
	skip_lines(history->fd, diff);

	for (size_t i = 0; i < HIST_SIZE; i++)
	{
		if (history->entries[i])
			free(history->entries[i]);
		history->entries[i] = get_next_line(history->fd);
		if (!history->entries[i])
		{
			get_next_line(-1);
			return false;
		}
		history->entries_sizes[i] = strlen(history->entries[i]);
		if (history->entries[i][history->entries_sizes[i] - 1] == '\n')
			history->entries[i][history->entries_sizes[i] - 1] = '\0';
	}
	get_next_line(-1);
	if (!diff)
		history->pos = history->pos_in_file;
	else
		history->pos = HIST_SIZE;
	return true;
}

bool	load_next_part_history(hist *history)
{
	lseek(history->fd, 0, SEEK_SET);
	skip_lines(history->fd, history->pos_in_file + 1);

	size_t max = (history->size - (history->pos_in_file + 1));
	if (max > HIST_SIZE)
		max = HIST_SIZE;

	for (size_t i = 0; i < max; i++)
	{
		free(history->entries[i]);
		history->entries[i] = get_next_line(history->fd);
		if (!history->entries[i])
		{
			get_next_line(-1);
			return false;
		}
		history->entries_sizes[i] = strlen(history->entries[i]);
		if (history->entries[i][history->entries_sizes[i] - 1] == '\n')
			history->entries[i][history->entries_sizes[i] - 1] = '\0';
	}
	for (size_t i = max; i < HIST_SIZE; i++)
	{
		if (history->entries[i])
		{
			free(history->entries[i]);
			history->entries[i] = NULL;
		}
	}
	get_next_line(-1);
	history->pos = -1;
	return true;
}

static int	get_offset(hist *history, off_t *line_start, off_t *line_end)
{
	size_t  i;
    off_t   pos;

    i = 0;
    pos = 0;
	lseek(history->fd, 0, SEEK_SET);
    while (i < history->size)
    {
		skip_lines(history->fd, 1);
		off_t next_pos = lseek(history->fd, 0, SEEK_CUR);
        if (i == history->pos_in_file)
        {
			*line_start = pos;
			*line_end = next_pos;
            return true;
        }
        pos = next_pos;
        i++;
    }
	return false;
}

void	paste_history(hist *history)
{
	if (history->on_curr)
	{
		if (lseek(history->fd, 0, SEEK_END) != 0)
			write(history->fd, "\n", 1);
		write(history->fd, history->curr_entry, strlen(history->curr_entry));
	}
	else
	{
		off_t line_start = 0, line_end = 0;
		if (!get_offset(history, &line_start, &line_end))
			return ;
		struct stat st;
		if (fstat(history->fd, &st) < 0 || st.st_size == 0)
			return ;
		char *map = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, history->fd, 0);
		if (map == MAP_FAILED)
			return ;
		memmove(map + line_start, map + line_end, st.st_size - line_end);
		msync(map, st.st_size, MS_SYNC);
		munmap(map, st.st_size);
		ftruncate(history->fd, line_start + st.st_size - line_end);
		lseek(history->fd, 0, SEEK_END);
		write(history->fd, "\n", 1);
		write(history->fd, history->entries[history->pos], strlen(history->entries[history->pos]));
	}
}

void	free_history(hist *history, bool flag)
{
	if (!flag)
		paste_history(history);
	for (size_t i = 0; i < HIST_SIZE; i++)
	{
		if (history->entries[i])
			free(history->entries[i]);
	}
	if (history->curr_entry)
			free(history->curr_entry);
	if (history->fd >= 0)
		close(history->fd);
}