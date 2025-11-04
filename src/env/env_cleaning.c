/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cleaning.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:41:15 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 16:42:41 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_str_arr(char ***words)
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
