/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:17:31 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/22 16:00:21 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// helper env_to_envp
int	build_envp(t_env *env, char **envp)
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

// destroy/free the env list
void	env_free(t_env **env)
{
	t_env	*tmp;
	t_env	*destroy;

	if (!env || !*env)
		return ;
	tmp = *env;
	while (tmp)
	{
		destroy = tmp;
		tmp = tmp->next;
		free(destroy->key);
		free(destroy->value);
		free(destroy);
	}
	*env = NULL;
}

void free_str_arr(char ***words)
{
	int	i;

	if (!words || !*words)
		return ;
	i = 0;
	while ((*words)[i])
	{
		free((*words)[i]);
		i++;
	}
	free(*words);
	*words = NULL;
}


