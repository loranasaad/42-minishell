/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:49:41 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/19 17:20:16 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// build the list with env variables (key, value)
t_env	*env_init(char **envp)
{
	int		i;
	t_env	*env;
	char	*key;
	char	*value;

	env = NULL;
	if (!envp || !envp[0])
	{
		build_min_env(&env);
		return (env);
	}
	i = 0;
	while (envp[i])
	{	
		key = get_key(envp[i]);
		value = ft_strchr(envp[i], '=');
		if (value)
			value = ft_strdup(value + 1);
		else
			value = ft_strdup("");
		if (add_env_var(&env, key, value) == 1)
		{
			env_free(&env);
			return (NULL);
		}
		i++;
	}
	handle_shlvl(&env);
	return (env);
}

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
int	add_env_var(t_env **env, char *key, char* value)
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
		{
			tmp = tmp->next;
		}
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
		if (tmp->key && strcmp(tmp->key, key) == 0)
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
	return (add_env_var(env, ft_strdup(key), ft_strdup(value))); // memory leaks if add_env_var fails
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
