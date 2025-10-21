/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 22:37:49 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// should not print exit in a pipeline

#include "minishell.h"

static int	is_numeric(char *arg);

int	builtin_exit(char **argv, t_ms *ms, int in_parent)
{
	if (in_parent)
		printf("exit\n");
	if (!argv[1])
		exit (ms->last_status);
	else if (argv[1] && !is_numeric(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit (2);
	}
	else if (argv[1] && argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	else
		exit ((unsigned char)ft_atoi(argv[1]));
}

static int	is_numeric(char *arg)
{
	int		i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] >= '0' && arg[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}
