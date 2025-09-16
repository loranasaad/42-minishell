/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:27:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/16 11:04:55 by latabagl         ###   ########.fr       */
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
char	*env_get(t_env *env, const char *key);
int		env_set(t_env **env, const char *key, const char *value, int overwrite);
int		env_unset(t_env **env, const char *key);
char	**env_to_envp(t_env *env);
int		builtin_env(t_env *env);
void	env_free(t_env *env);

// debug
void env_print_debug(t_env *env);


#endif