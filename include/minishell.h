/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:27:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 13:17:15 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct	s_env	*next;
}					t_env;

typedef struct s_ms
{
	t_env	*env;
	int		last_status;
	int		interactive;
}			t_ms;

void	replt(t_ms *ms);

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
int		builtin_env(t_env *env);
void	env_free(t_env **env);
void	handle_shlvl(t_env **env);
void	build_min_env(t_env **env);


// debug
void	env_print_debug(t_env *env);
size_t	len_str_arr(char **words);

// utils
void	free_str_arr(char ***words);

// utils libft
char			*ft_strdup(const char *s);
size_t			ft_strlen(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strchr(const char *s, int c);
char			*ft_substr(char const *s, unsigned int start, size_t len);
void			*ft_memcpy(void *dest, const void *src, size_t n);
int				ft_atoi(const char *str);
int				ft_isspace(char c);
char			*ft_itoa(int n);
unsigned int	ft_how_many_digits(int n);
int				ft_handle_neg(int *n);

#endif