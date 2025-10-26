/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/26 18:30:58 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>

static int	is_numeric(char *arg);

int	builtin_exit(char **argv, t_ms *ms, int in_parent)
{
	int	exit_code;						//leaks
	
	if (in_parent)
		printf("exit\n");
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
		return (1);
	}
	else
		exit_code = (unsigned char)ft_atoi(argv[1]);
	
	ms->last_status = exit_code;	//leaks
	if (in_parent)					//leaks
	{
		ms->exit_requested = 1;
		return (exit_code);
	}
	else							//leaks
	{
		clear_history();
		env_free(&(ms->env));
		exit(exit_code);
	}
	
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
