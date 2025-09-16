NAME = minishell

CC = cc
CFLAGS = -Iinclude

SRCS = src/main.c src/repl.c src/debug.c src/builtin.c src/env_list.c src/utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all