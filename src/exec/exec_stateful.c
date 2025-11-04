/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stateful.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:56:24 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:14:36 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>

static int	save_std(int std_backup[2])
{
	std_backup[0] = dup(STDIN_FILENO);
	if (std_backup[0] < 0)
		return (0);
	std_backup[1] = dup(STDOUT_FILENO);
	if (std_backup[1] < 0)
	{
		close(std_backup[0]);
		return (0);
	}
	return (1);
}

static void	restore_std(int std_backup[2])
{
	if (std_backup[0] >= 0)
	{
		dup2(std_backup[0], STDIN_FILENO);
		close(std_backup[0]);
	}
	if (std_backup[1] >= 0)
	{
		dup2(std_backup[1], STDOUT_FILENO);
		close(std_backup[1]);
	}
}

int	exec_stateful(t_cmdspec *spec, t_ms *ms)
{
	int	std_backup[2];
	int	rc;

	if (spec->argv && spec->argv[0]
		&& ft_strcmp(spec->argv[0], "exit") == 0)
	{
		builtin_dispatch(spec->argv, ms, &rc, 1);
		return (rc);
	}
	if (!save_std(std_backup))
		return (1);
	if (!apply_redirs(spec->redirs))
	{
		restore_std(std_backup);
		return (1);
	}
	rc = 0;
	builtin_dispatch(spec->argv, ms, &rc, 1);
	restore_std(std_backup);
	return (rc);
}
