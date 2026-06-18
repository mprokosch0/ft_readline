#ifndef FT_READLINE_H
# define FT_READLINE_H

# include <termios.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <fcntl.h>

# define HIST_SIZE 25

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# define isPrint(c) (c >= 32 && c <= 126)

# define UP_ARR		"\033[A"
# define DOWN_ARR	"\033[B"
# define RIGHT_ARR	"\033[C"
# define LEFT_ARR	"\033[D"
# define HOME		"\033[H"
# define END		"\033[F"

typedef struct hist
{
	size_t		entries_sizes[HIST_SIZE];
	char		*entries[HIST_SIZE];
	char		*curr_entry;
	size_t		curr_entry_size;
	size_t		pos;
	size_t		pos_in_file;
	size_t		size;
	int			fd;
	int			on_curr;
}hist;


// utils

char	*get_next_line(int fd);
char	*ft_strjoin(char const *s1, char const *s2);
void	skip_lines(int fd, size_t nb_lines);

//read

int		ft_read(hist *current);

// history

void	load_history(hist *history);
void	load_prev_part_history(hist *history);
void	load_next_part_history(hist *history);
void	free_history(hist *history);


void	init_termios(struct termios *new1, struct termios *old);
void	reset_termios(struct termios *old);

#endif