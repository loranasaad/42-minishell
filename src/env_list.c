/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:49:41 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/16 11:16:38 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// build the list with env variables (key, value)
t_env	*env_init(char **envp)
{
	return (NULL);
}

// if key found, returns value, else NULL
char	*env_get(t_env *env, const char *key)
{
	return (NULL);
}

// Key exists => overwrite or not the value. No key => add new node 
int	env_set(t_env **env, const char *key, const char *value, int overwrite)
{
	return (0);
}
// find the key and remove the node, return 1 if key not found
int	env_unset(t_env **env, const char *key)
{
	return (0);
}

// list => array (to be passed to execve) 
char	**env_to_envp(t_env *env)
{
	return (NULL);
}

// destroy/free the env list
void	env_free(t_env *env)
{
	return ;
}