/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:12:18 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/19 12:37:13 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print everything in the env list
void	env_print_debug(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp)
	{
		printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

size_t	len_str_arr(char **words)
{
	size_t	i;

	i = 0;
	if (!words)
		return (0);
	while (words[i])
	{
		i++;
	}
	return (i);
}