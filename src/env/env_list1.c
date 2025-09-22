/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:17:24 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/22 15:20:41 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// build the list with env variables (key, value)
t_env	*env_init(char **envp)
{
	t_env	*env;

	env = NULL;
	if (!envp || !envp[0])
	{
		build_min_env(&env);
		return (env);
	}
	fill_env(envp, &env);
	handle_shlvl(&env);
	return (env);
}

// helper for env_init
void	fill_env(char **envp, t_env	**env)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		key = get_key(envp[i]);
		value = ft_strchr(envp[i], '=');
		if (value)
			value = ft_strdup(value + 1);
		else
			value = ft_strdup("");
		if (add_env_var(env, key, value) == 1)
		{
			env_free(env);
			exit (1); // malloc fails => stop there
		}
		i++;
	}
}

// helper for env_init
char	*get_key(char *env_var)
{
	size_t	i;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
	{
		i++;
	}
	return (ft_substr(env_var, 0, i));
}

// add an env var to the list, val must never be NULL but "" if nothing
int	add_env_var(t_env **env, char *key, char *value)
{
	t_env	*var;
	t_env	*tmp;

	var = malloc(sizeof(t_env));
	if (!var || !key || !value)
	{
		free(value);
		free(key);
		free(var);
		return (1);
	}
	var->key = key;
	var->value = value;
	var->next = NULL;
	if (!*env)
		*env = var;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = var;
	}
	return (0);
}

void	build_min_env(t_env **env)
{
	env_set(env, "SHLVL", "1", 0);
	env_set(env, "PWD", getcwd(NULL, 0), 0);
	env_set(env, "PATH", "/usr/bin:/bin", 0);
}
