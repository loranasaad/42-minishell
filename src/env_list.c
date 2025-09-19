/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:49:41 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/18 22:14:38 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// build the list with env variables (key, value)
t_env	*env_init(char **envp)
{
	int		i;
	t_env	*env;
	char	**key_value;
	char	*key;
	char	*value;

	//  !envp shouldnt happen, envp[0] = NULL when env -i ./minishell
	if (!envp || !envp[0])
	{
		return (NULL); // build minimal env
	}
	env = NULL;
	i = 0;
	while (envp[i])
	{
		key_value =  ft_split(envp[i], '=');
		if (!key_value) // check that length is 2
			continue ;
		key = ft_strdup(key_value[0]); // !key
		value = ft_strdup(key_value[1]); // !value
		add_env_var(&env, key, value); // what if that fails ?
		free_str_arr(&key_value);
		i++;
	}
	return (env);
}

int	add_env_var(t_env **env, char *key, char* value)
{
	t_env	*var;
	t_env	*tmp;
	
	var = malloc(sizeof(t_env));
	if (!var)
		return (1);
	var->key = key;
	var->value = value;
	var->next = NULL;	
	if (!*env)
		*env = var;
	else
	{
		tmp = *env;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = var;
	}
	return (0);
}

// if key found, returns value, else NULL !!! CASE SENSITIVE
char	*env_get(t_env *env, char *key)
{
	t_env	*tmp;

	if (!key)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (tmp->key && strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

// Key exists => overwrite or not the value. No key => add new node 
int	env_set(t_env **env, char *key, char *value, int overwrite)
{
	t_env	*tmp;

	if (!key || !env)
		return (1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->key && strcmp(tmp->key, key) == 0)
		{
			if (overwrite)
			{
				free(tmp->value);
				if (value)
					tmp->value = ft_strdup(value);
				else
					tmp->value = NULL;
				if (value && !tmp->value)
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
		if (w->key && strcmp(w->key, key) == 0)
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

// list => array (to be passed to execve) 
char	**env_to_envp(t_env *env)
{
	t_env	*w;
	int		i;
	char	**envp;
	char	*tmp;

	i = 0;
	w = env;
	while (w)
	{
		i++;
		w = w->next;
	}
	envp = malloc((i + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	w = env;
	i = 0;
	while (w)
	{
		tmp = ft_strjoin(w->key, "=");
		if (!tmp)
		{
			envp[i] = NULL;
			free_str_arr(&envp);
			return (NULL);
		}
		if (w->value)
			envp[i] = ft_strjoin(tmp, w->value);
		else
			envp[i] = ft_strdup(tmp);
		free(tmp);
		if (!envp[i])
		{
			free_str_arr(&envp);
			return (NULL);
		}
		w = w->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// destroy/free the env list
void	env_free(t_env **env)
{
	t_env	*tmp;
	t_env	*destroy;

	if (!env || !*env)
		return;
	tmp = *env;
	while (tmp)
	{
		destroy = tmp;
		tmp=tmp->next;
		free(destroy->key);
		free(destroy->value);
		free(destroy);
	}
	*env = NULL;
}
