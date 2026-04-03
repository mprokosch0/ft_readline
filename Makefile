NAME = readline

CC = cc
FLAGS = -Wall -Wextra -Werror -Iincludes -g

P1 = srcs/main.c srcs/manage_term.c srcs/hist.c srcs/read.c

OBJS = $(P1:.c=.o)

all: $(NAME)

bonus: $(NAME) $(NAME2)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(FLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: fclean all clean re 