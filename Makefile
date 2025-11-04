NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinclude
LDFLAGS =
LDLIBS  =

SRCS = \
	src/main.c \
	src/utils.c \
	\
	src/repl/repl.c \
	src/repl/repl_utils.c \
	\
	src/signals/signals_exec.c \
	src/signals/signals_prompt.c \
	\
	src/lex/lex.c \
	src/lex/lex_copy.c \
	src/lex/lex_read.c \
	src/lex/lex_utils.c \
	src/lex/tok_utils.c \
	\
	src/parser/parser_cleanup.c \
	src/parser/parse_line.c \
	src/parser/parser_validate.c \
	src/parser/build_ast.c \
	src/parser/cmdspec.c \
	src/parser/cmdspec_redir.c \
	src/parser/cmdspec_heredoc.c \
	src/parser/cmdspec_utils.c \
	\
	src/env/build_env_list.c \
	src/env/env_cleaning.c \
	src/env/env_list_utils.c \
	src/env/env_to_envp.c \
	src/env/find_in_path.c \
	\
	src/exec/exec_child_utils.c \
	src/exec/exec_child.c \
	src/exec/exec_node.c \
	src/exec/exec_one.c \
	src/exec/exec_path.c \
	src/exec/exec_pipeline_cleanup.c \
	src/exec/exec_pipeline_setup.c \
	src/exec/exec_pipeline.c \
	src/exec/exec_stateful.c \
	src/exec/exec_utils.c \
	src/exec/hdoc_prep.c \
	src/exec/hdoc_fill.c \
	src/exec/hdoc_signal.c \
	src/exec/redir.c \
	\
	src/expand/var_expansion.c \
	src/expand/var_expansion_utils.c \
	src/expand/tilde_expansion.c \
	src/expand/field_split.c \
	\
	src/builtin/builtin_dispatch.c \
	src/builtin/cd.c \
	src/builtin/echo.c \
	src/builtin/env.c \
	src/builtin/exit.c \
	src/builtin/export.c \
	src/builtin/export_utils.c \
	src/builtin/export_print.c \
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
  BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
  RL_PREFIX   := $(shell brew --prefix readline 2>/dev/null)

  ifeq ($(RL_PREFIX),)
    RL_PREFIX := /opt/homebrew/opt/readline
  endif

  CFLAGS  += -I$(RL_PREFIX)/include
  LDFLAGS += -L$(RL_PREFIX)/lib
  LDLIBS  += -lreadline
else
  # Linux
  LDLIBS  += -lreadline -lhistory -lncurses
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ==============================
# Valgrind Leak Test
# ==============================

SUPP_FILE = valgrind_readline.supp

leaktest: $(NAME)
	@echo "🔍 Running Valgrind leak test (readline leaks suppressed)..."
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --track-fds=yes \
		--suppressions=$(SUPP_FILE) ./$(NAME)

.PHONY: all clean fclean re leaktest