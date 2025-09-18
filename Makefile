NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinclude
LDFLAGS =
LDLIBS  =

SRCS = src/main.c src/repl.c src/signals.c
OBJS = $(SRCS:.c=.o)

# Detect OS to set readline include/lib paths and extra libs
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  # macOS (Homebrew)
  # If brew isn't installed or readline not found, hardcode /opt/homebrew path
  BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
  RL_PREFIX   := $(shell brew --prefix readline 2>/dev/null)

  # Fallbacks if brew --prefix returns nothing
  ifeq ($(RL_PREFIX),)
    RL_PREFIX := /opt/homebrew/opt/readline
  endif

  CFLAGS  += -I$(RL_PREFIX)/include
  LDFLAGS += -L$(RL_PREFIX)/lib
  LDLIBS  += -lreadline
else
  # Linux
  LDLIBS  += -lreadline -lncurses
  # If your distro uses tinfo instead of ncurses, swap the previous line for:
  # LDLIBS  += -lreadline -ltinfo
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all