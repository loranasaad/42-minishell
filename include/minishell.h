/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:27:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/16 14:51:48 by latabagl         ###   ########.fr       */
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

// environment list
t_env	*env_init(char **envp);
int		add_env_var(t_env **env, char *key, char* value);
char	*env_get(t_env *env, char *key);
int		env_set(t_env **env, char *key, char *value, int overwrite);
int		env_unset(t_env **env, char *key);
char	**env_to_envp(t_env *env);
int		builtin_env(t_env *env);
void	env_free(t_env **env);

// debug
void	env_print_debug(t_env *env);

// utils
void	free_str_arr(char ***words);

// utils libft
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
size_t	countwords(char const *s, char c);
void	find_and_allocate_words(char const *s, char c, char **w, size_t w_i);
char	*allocate_word(char const *s, size_t begin, size_t end);
void	free_words(char **words, size_t count);

#endif