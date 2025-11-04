/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:17:31 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 16:48:31 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// helper env_to_envp
static int	build_envp(t_env *env, char **envp)
{
	t_env	*w;
	int		i;
	char	*tmp;

	w = env;
	i = 0;
	while (w)
	{
		tmp = ft_strjoin(w->key, "=");
		if (!tmp)
			return (1);
		if (w->value)
			envp[i] = ft_strjoin(tmp, w->value);
		else
			envp[i] = ft_strdup(tmp);
		free(tmp);
		if (!envp[i])
			return (1);
		w = w->next;
		i++;
	}
	envp[i] = NULL;
	return (0);
}

// list => array (to be passed to execve) 
char	**env_to_envp(t_env *env)
{
	t_env	*w;
	int		i;
	char	**envp;

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
	if (build_envp(env, envp) == 1)
	{
		free_str_arr(&envp);
		return (NULL);
	}
	return (envp);
}
