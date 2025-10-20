/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:18 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 15:36:36 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export a=1 b= c I see b= with bash and not with minishell

int	builtin_env(char **argv, t_ms *ms)
{
	t_env	*w;

	if (argv[1])
	{
		write(2, "env: ", 5);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": No such file or directory\n", 28);
		return (127);
	}
	w = ms->env;
	while (w)
	{
		if (w->key && w->value && w->value[0])
			printf("%s=%s\n", w->key, w->value);
		w = w->next;
	}
	return (0);
}
