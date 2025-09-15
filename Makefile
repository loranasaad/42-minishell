NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

SRCS = src/main.c src/repl.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -lreadline -o $@ $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all