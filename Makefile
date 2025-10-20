NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinclude
LDFLAGS =
LDLIBS  =

SRCS = \
	src/main.c \
	src/repl.c \
	src/signals.c \
	src/termios_helpers.c \
	src/utils.c \
	\
	src/lex/lex.c \
	src/lex/lex_utils.c \
	src/lex/tok_utils.c \
	\
	src/parser/parse_line.c \
	src/parser/validate.c \
	src/parser/ast_free.c \
	src/parser/parser_build.c \
	src/parser/cmdspec1.c \
	src/parser/cmdspec2.c \
	src/parser/cmdspec3.c \
	src/parser/var_expansion.c \
	src/parser/var_expansion2.c \
	src/parser/field_split.c \
	\
	src/env/env_list1.c \
	src/env/env_list2.c \
	src/env/env_list3.c \
	src/env/path.c \
	\
	src/exec/exec_one.c \
	src/exec/redir.c \
	src/exec/exec_node.c \
	src/exec/exec_pipeline.c \
	\
	src/builtin/cd.c \
	src/builtin/echo.c \
	src/builtin/env.c \
	src/builtin/exit.c \
	src/builtin/export.c \
	src/builtin/pwd.c \
	src/builtin/unset.c \
	\
	src/ft_lib/ft_atoi.c \
	src/ft_lib/ft_itoa.c \
	src/ft_lib/ft_memcpy.c \
	src/ft_lib/ft_strchr.c \
	src/ft_lib/ft_strdup.c \
	src/ft_lib/ft_strjoin.c \
	src/ft_lib/ft_strlen.c \
	src/ft_lib/ft_substr.c \
	src/ft_lib/ft_split.c \
	src/ft_lib/ft_strcmp.c
  
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
  LDLIBS  += -lreadline -lhistory -lncurses
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