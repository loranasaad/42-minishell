/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:17:24 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 17:09:09 by latabagl         ###   ########.fr       */
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

// helper for env_init, here I have exit for malloc fail
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
			exit (1);
		}
		i++;
	}
}

// Key exists => overwrite or not the value. No key => add new node 
int	env_set(t_env **env, char *key, char *value, int overwrite)
{
	t_env	*tmp;

	if (!key || !env)
		return (1);
	if (!value)
		value = "";
	tmp = *env;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, key) == 0)
		{
			if (overwrite)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
				if (!tmp->value)
					return (1);
			}
			return (0);
		}
		tmp = tmp->next;
	}
	return (add_env_var(env, ft_strdup(key), ft_strdup(value)));
}

// find the key and remove the node, return 1 if key not found
int	env_unset(t_env **env, char *key)
{
	t_env	*w;
	t_env	*before;

	if (!key || !env)
		return (1);
	before = NULL;
	w = *env;
	while (w)
	{
		if (w->key && ft_strcmp(w->key, key) == 0)
		{
			if (before)
				before->next = w->next;
			else
				*env = w->next;
			free(w->key);
			free(w->value);
			free(w);
			return (0);
		}
		before = w;
		w = w->next;
	}
	return (1);
}
