/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 16:58:07 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>

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

static int	get_exit_code(char **argv, t_ms *ms, int *r)
{
	int	exit_code;

	if (!argv[1])
		exit_code = (ms->last_status);
	else if (argv[1] && !is_numeric(argv[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": numeric argument required\n", 28);
		exit_code = 2;
	}
	else if (argv[1] && argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		*r = 1;
		exit_code = 1;
	}
	else
		exit_code = (unsigned char)ft_atoi(argv[1]);
	return (exit_code);
}

int	builtin_exit(char **argv, t_ms *ms, int in_parent)
{
	int	exit_code;
	int	r;

	if (in_parent)
		printf("exit\n");
	r = 0;
	exit_code = get_exit_code(argv, ms, &r);
	if (r)
		return (1);
	ms->last_status = exit_code;
	if (in_parent)
	{
		ms->exit_requested = 1;
		return (exit_code);
	}
	else
	{
		clear_history();
		env_free(&(ms->env));
		exit(exit_code);
	}
}
