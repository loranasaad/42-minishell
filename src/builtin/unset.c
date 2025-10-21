/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:31 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 16:53:30 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **argv, t_ms *ms)
{
	int		i;

	i = 1;
	while (argv[i])
	{
		env_unset(&ms->env, argv[i]);
		i++;
	}
	return (0);
}
