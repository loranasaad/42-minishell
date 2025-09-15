// cc test2.c -Llibft -lreadline -lft
#include "libft/includes/libft.h"
#include <stdio.h>
# include <readline/readline.h> // readline
# include <readline/history.h> // add_history

typedef enum e_token_type
{
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	META
}			t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_builtin
{
const char *builtin_name;
int (*foo)(char **);

} t_builtin;

int		is_metacharacter(char c);
t_token	*token_new(t_token_type type, char *value);
void	tokens_add_back(t_token **tokens, t_token *new);
void	self_destruction(void);
void	get_token(t_token **tokens, int begin, int end, char *str, t_token_type type);
void	print_tokens(t_token *tokens);
void	parse_line(char *str, t_token **tokens);
void	free_tokens(t_token **tokens);
int 	cell_echo(char **args);
int 	cell_exit(char **args);
void	free_env_var(t_env **env_var);
void 	free_split(char **words);
void	cell_env(t_env *env_var);
void	add_env_var(t_env **env_var, char *key, char* value);


t_builtin g_builtin[] =
{
{.builtin_name="echo", .foo=cell_echo},
{.builtin_name="exit", .foo=cell_exit},
//{.builtin_name="env", .foo=cell_env},
{.builtin_name=NULL}
};

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;

	int		i;
	char	**key_value;
	t_env	*env_var;
	char	*key;
	char	*value;

	env_var = NULL;
	if (!envp)
	{
		printf("envp is NULL\n");
		return (1);
	}
	i = 0;
	while (envp[i])
	{
		key_value =  ft_split(envp[i], '='); // several =
		if (!key_value) // NULL
			continue ; // if no =, only one element in split
		key = ft_strdup(key_value[0]); // !key
		value = ft_strdup(key_value[1]); // !value
		add_env_var(&env_var, key, value);
		free_split(key_value);
		i++;
	}
	//cell_env(env_var);

	t_token	*tokens;
	char	*line;

	tokens = NULL;
	while (1)
	{
		line = readline("BabyCell$ ");
		if (!line)
			return (1);
		add_history(line);
		parse_line(line, &tokens);
		print_tokens(tokens);
		free(line);
		free_tokens(&tokens);
	}	
	rl_clear_history();
	free_env_var(&env_var);
}

void	parse_line(char *str, t_token **tokens)
{
	int	inside_token = 0;
	int	inside_quote = 0;
	int	inside_word = 0;
	int	begin;
	int	end;

	int i = 0;
	while (str[i])
	{
		if (!inside_token)
		{
			if (str[i] == ' ')
				;
			else if (ft_isalnum(str[i]) || str[i] == '$' || str[i] == '/' || str[i] == '-')
			{
				inside_word = 1;
				inside_token = 1;
				begin = i;
			}
			else if (str[i] == '\'' || str[i] == '"')
			{
				inside_quote = 1;
				inside_token = 1;
				begin = i + 1;
			}
			else if (is_metacharacter(str[i]))
			{
				begin = i;
				if (is_metacharacter(str[i + 1]))
				{
					i++;
				}
				end = i + 1;
				get_token(tokens, begin, end, str, META);
			}
		}
		else if (inside_quote)
		{
			if (str[i] == '\'' || str[i] == '"')
			{
				inside_token = 0;
				inside_quote = 0;
				end = i;
				if (str[i] == '\'')
					get_token(tokens, begin, end, str, SINGLE_QUOTE);
				else
					get_token(tokens, begin, end, str, DOUBLE_QUOTE);
			}
			else
			{
				;
			}
		}
		else if (inside_word)
		{
			if (ft_isalnum(str[i]) || str[i] == '$' || str[i] == '/' || str[i] == '-')
			{
				;
			}
			else 
			{
				inside_word = 0;
				inside_token = 0;
				end = i;
				get_token(tokens, begin, end, str, WORD);
			}
		}
		i++;
	}
	if (inside_word)
	{
		end = i;
		get_token(tokens, begin, end, str, WORD);
	}
}

int		is_metacharacter(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

t_token	*token_new(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	tokens_add_back(t_token **tokens, t_token *new)
{
	t_token	*w;

	w = *tokens;
	if (!w)
	{
		*tokens = new;
		return ;
	}
	while (w->next)
	{
		w = w->next;
	}
	w->next = new;
}

void	get_token(t_token **tokens, int begin, int end, char *str, t_token_type type)
{
	char	*value = malloc(end - begin + 1);
	if (!value)
		self_destruction();
	int	i = 0;
	while (begin < end)
		value[i++] = str[begin++];
	value[i] = '\0';
	t_token *token = token_new(type, value);
	tokens_add_back(tokens, token);
}

void	self_destruction(void)
{
	exit(1);
}

void	print_tokens(t_token *tokens)
{
	t_token *w = tokens;
	while (w)
	{
		printf("Token ! ");
		printf("Value : %s  ", w->value);
		if (w->type == WORD)
			printf("Type Word\n");
		else if (w->type == SINGLE_QUOTE)
			printf("Type Single Quote\n");
		else if (w->type == DOUBLE_QUOTE)
			printf("Type Double Quote\n");
		else if (w->type == META)
			printf("Type Meta\n");
		w = w->next;
	}
}

void	free_tokens(t_token **tokens)
{
	t_token *w;
	t_token *tmp;

	if (!tokens || !*tokens)
		return ;
	w = *tokens;
	while (w)
	{
		tmp = w;
		w = w->next;
		free(tmp->value);
		free(tmp);
	}
	*tokens = NULL;
}

int cell_echo(char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		printf(" ");
		i++;
	}

	printf("\n");
	return (0);
}

int cell_exit(char **args)
{
	(void) args;
	// free memory
	exit(0);
}

void	add_env_var(t_env **env_var, char *key, char* value)
{
	t_env	*var;
	t_env	*tmp;
	
	var = malloc(sizeof(t_env));
	if (!var)
	{
		printf("oups\n");
		return ;
	}
	
	var->key = key;
	var->value = value;
	var->next = NULL;
	
	if (!*env_var)
		*env_var = var;
	else
	{
		tmp = *env_var;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = var;
	}
}

void	cell_env(t_env *env_var)
{
	t_env	*tmp;

	if (!env_var)
		return ;
	tmp = env_var;
	while (tmp)
	{
		// what if something is NULL
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

void	shell_export(void)
{
	;
}

void	shell_unset(void)
{
	;
}

void free_split(char **words)
{
	int	i;

	if (!words || !*words)
		return ;
	i = 0;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}

void	free_env_var(t_env **env_var)
{
	t_env	*tmp;
	t_env	*destroy;

	if (!env_var || !*env_var)
		return;
	tmp = *env_var;
	while (tmp)
	{
		destroy = tmp;
		tmp=tmp->next;
		free(destroy->key);
		free(destroy->value);
		free(destroy);
	}
	*env_var = NULL;
}
