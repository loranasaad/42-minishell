/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:48:15 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 16:48:19 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if key found, returns value, else NULL !!! CASE SENSITIVE
char	*env_get(t_env *env, char *key)
{
	t_env	*tmp;

	if (!key)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	handle_shlvl(t_env	**env)
{
	char	*shell_lvl;
	int		shell_lvl_nb;

	shell_lvl = env_get(*env, "SHLVL");
	if (!shell_lvl)
	{
		env_set(env, "SHLVL", "1", 0);
		return ;
	}
	shell_lvl_nb = ft_atoi(shell_lvl);
	shell_lvl_nb++;
	if (shell_lvl_nb > 1000 || shell_lvl_nb < 0)
		shell_lvl_nb = 1;
	shell_lvl = ft_itoa(shell_lvl_nb);
	if (!shell_lvl)
		return ;
	env_set(env, "SHLVL", shell_lvl, 1);
	free(shell_lvl);
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
