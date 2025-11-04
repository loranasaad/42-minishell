/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:48:15 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 17:09:13 by latabagl         ###   ########.fr       */
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

void	build_min_env(t_env **env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	env_set(env, "SHLVL", "1", 0);
	env_set(env, "PWD", cwd, 0);
	env_set(env, "PATH", "/usr/bin:/bin", 0);
	free(cwd);
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
	var->has_value = 1;
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
