/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_cleanup.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:03:10 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:10:27 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"

void	pipeline_cleanup(t_pipe_data *data, int flags)
{
	if ((flags & CU_CLOSE_PIPES) && data->pipes)
		close_pipes(data->pipes, data->len);
	if (flags & CU_HDOCS)
		hdoc_cleanup_specs(data->specs, data->len);
	if (flags & CU_SPECS)
		free_specs(data->specs, data->len);
	if (flags & CU_STAGES)
		free(data->stages);
	if (flags & CU_PIPES)
		free(data->pipes);
	if (flags & CU_PIDS)
		free(data->pids);
}

void	close_pipes(int (*pipes)[2], int len)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < len - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	hdoc_cleanup(t_redir *r)
{
	t_redir	*current;

	current = r;
	while (current)
	{
		if (current->kind == R_HDOC && current->hdoc_fd >= 0)
		{
			close(current->hdoc_fd);
			current->hdoc_fd = -1;
		}
		current = current->next;
	}
}

void	hdoc_cleanup_specs(t_cmdspec *s, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		hdoc_cleanup(s[i].redirs);
		i++;
	}
}

void	free_specs(t_cmdspec *specs, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		free_cmdspec(&specs[i]);
		i++;
	}
	free(specs);
}
