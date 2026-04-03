#ifndef FT_READLINE_H
# define FT_READLINE_H

# include <termios.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# define SIZE 4096 

# define isPrint(c) (c >= 32 && c <= 126)

# define UP_ARR		"\033[A"
# define DOWN_ARR	"\033[B"
# define RIGHT_ARR	"\033[C"
# define LEFT_ARR	"\033[D"
# define HOME		"\033[H"
# define END		"\033[F"

typedef struct hist
{
	char		*buffer;
	size_t		size;
	struct hist	*next;
	struct hist	*prev;
}hist;


//read
int		ft_read(hist **current);

//double linked list

hist	*new_node(void);
void	delete_node(hist **head, hist **tail, hist *node);
void	delete_hist(hist **head);
void	move_to_last(hist **head, hist **tail, hist *node);
void	static_infos(hist **head, bool mode);
bool	hist_add_back(hist **head, hist **tail, hist *new);
bool	hist_add_front(hist **head, hist **tail, hist *new);

void	init_termios(struct termios *new1, struct termios *old);
void	reset_termios(struct termios *old);

#endif