
#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "parser.h"

typedef	enum	e_tok
{
	TK_WORD,
	TK_PIPE,	// |
	TK_IN,		// <
	TK_OUT,		// >
	TK_APP,		// >>
	TK_HDOC,	// <<
}				t_tok;

typedef struct s_token
{
	t_tok			kind;
	char			*val;
	int				quoted;
	struct	s_token	*next;
}					t_token;


typedef struct s_env
{
	char			*key;
	char			*value;
	int				has_value; // new thing for export and env !!!
	struct	s_env	*next;
}					t_env;

typedef struct s_ms
{
	t_env	*env;
	int		last_status;
	int		interactive;
	int		exit_requested;	//leak
}			t_ms;

typedef	struct s_cu		//leak cleanup
{
	t_ast		*ast;
	t_token		*toks;
	t_cmdspec	*spec;
}				t_cu;

#define IFS " \t\n"

extern	int	g_signal;

void	repl(t_ms *ms);

void	init_prompt_signals(void);

//int		termios_disable_echoctl(void);
//void	termios_restore(void);

t_token	*lex_line(const char *line, int *lex_status);

int		match_2(const char *s, int i, char a, char b);
int		is_meta(char c);
int		is_space(char c);
int		word_len(char *str, int *i);

t_token	*tok_new(t_tok kind, char *val, int quoted);
void	tok_push_back(t_token **head, t_token *node);
void	free_tokens(t_token *t);

//utils
void	ms_perror(const char *prefix, const char *arg);

// environment list
t_env	*env_init(char **envp);
void	fill_env(char **envp, t_env	**env);
int		add_env_var(t_env **env, char *key, char* value);
char	*get_key(char *env_var);
char	*env_get(t_env *env, char *key);
int		env_set(t_env **env, char *key, char *value, int overwrite);
int		env_unset(t_env **env, char *key);
char	**env_to_envp(t_env *env);
int		build_envp(t_env *env, char **envp);
void	env_free(t_env **env);
void	handle_shlvl(t_env **env);
void	build_min_env(t_env **env);
void	free_str_arr(char ***words);

char	*find_in_path(char const *name, t_env *env);
int		build_cmdspec_from_segment(t_token *start, t_token *end, t_cmdspec *out, t_ms *ms);
t_redir	*build_tredir(t_token *tok, t_ms *ms);
void	free_cmdspec(t_cmdspec *spec);
char	**strv_push(char **v, const char *s);

// ft_lib
int		ft_atoi(const char *str);
char	*ft_itoa(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *src);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strcmp(const char *s1, const char *s2); //this one is not part of libft
char	**ft_split(char const *s, char c);

// env
char	*find_in_path(const char *name, t_env *env);

// builtin
t_redir	*build_heredoc(t_token *tok);
void	builtin_dispatch(char **argv, t_ms *ms, int *rc, int print);
int		builtin_echo(char **argv);
int		builtin_pwd(char **argv);
int		builtin_cd(char **argv, t_ms *ms);
int		builtin_env( char **argv, t_ms *ms);
int		builtin_unset(char **argv, t_ms *ms);
int		builtin_exit(char **argv, t_ms *ms, int in_parent);
int		builtin_export(char **argv, t_ms *ms);
// export helpers
int		export_print_env(t_ms *ms);
int		env_set_export(t_env **env, char *key, char *value, int has_value);
int		is_key_valid(char *key);

// var expansion
char	*handle_var_expansion(char *str, int quoted, t_ms *ms, int *status);
char	*get_next_segment(char *str, int *i, t_ms *ms, int *end);
char	*expand_dollar_sign(char *str, t_ms *ms);
int		several_fields(int quoted, char	*val);
int		field_split(char *val, t_cmdspec *out);
char	*expand_tilde(char *str, int quoted, t_ms *ms);

#endif