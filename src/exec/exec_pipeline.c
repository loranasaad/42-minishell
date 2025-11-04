/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:47 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:14:07 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

static	int	wait_pipeline(pid_t *pids, int len)
{
	int	i;
	int	status;
	int	rc;

	rc = 0;
	i = 0;
	while (i < len)
	{
		while (waitpid(pids[i], &status, 0) < 0)
		{
			if (errno != EINTR)
				break ;
		}
		if (i == len - 1)
			rc = status_to_rc(status);
		i++;
	}
	return (rc);
}

static	int	fork_pipeline_children(t_pipe_data *data, t_ms *ms, t_cu *cleanup)
{
	int				i;

	i = 0;
	while (i < data->len)
	{
		data->pids[i] = fork();
		if (data->pids[i] < 0)
		{
			ms_perror("minishell", "fork");
			return (0);
		}
		else if (data->pids[i] == 0)
		{
			prep_cleanup(data, cleanup, i);
			set_child_pipes(i, data, ms, cleanup);
			close_pipes(data->pipes, data->len);
			exec_child_command(&data->specs[i], ms, cleanup);
		}
		i++;
	}
	return (1);
}

static	int	setup_pipeline(t_ast *root, t_pipe_data *data, t_ms *ms)
{
	data->len = count_pipeline(root);
	data->stages = malloc(data->len * sizeof(*data->stages));
	if (!data->stages)
		return (1);
	flatten_pipeline(root, data->stages, data->len);
	data->specs = malloc(sizeof(*data->specs) * data->len);
	if (!data->specs)
	{
		free(data->stages);
		return (1);
	}
	if (!build_all_specs(data->stages, data->len, data->specs, ms))
	{
		free(data->stages);
		return (2);
	}
	return (0);
}

static	int	setup_pipes_and_fork(t_pipe_data *data, t_ms *ms, t_cu *cleanup)
{
	data->pipes = malloc(sizeof(*data->pipes) * (data->len - 1));
	if (!data->pipes)
	{
		pipeline_cleanup(data, CU_BASIC);
		return (0);
	}
	if (!make_pipes(data->pipes, data->len))
	{
		pipeline_cleanup(data, CU_PIPES | CU_HDOCS | CU_SPECS | CU_STAGES);
		return (0);
	}
	data->pids = malloc(sizeof(pid_t) * data->len);
	if (!data->pids)
	{
		pipeline_cleanup(data, CU_FULL_PIPES);
		return (0);
	}
	if (!fork_pipeline_children(data, ms, cleanup))
	{
		pipeline_cleanup(data, CU_ALL);
		return (0);
	}
	return (1);
}

int	exec_pipeline(t_ast *root, t_ms *ms, t_cu *cleanup)
{
	int			rc;
	int			hdoc_rc;
	t_pipe_data	data;

	rc = setup_pipeline(root, &data, ms);
	if (rc != 0)
		return (rc);
	hdoc_rc = prepare_all_heredocs(data.specs, data.len, ms);
	if (hdoc_rc != 0)
	{
		pipeline_cleanup(&data, CU_BASIC);
		return (hdoc_rc);
	}
	if (!setup_pipes_and_fork(&data, ms, cleanup))
		return (1);
	close_pipes(data.pipes, data.len);
	init_exec_signals();
	rc = wait_pipeline(data.pids, data.len);
	init_prompt_signals();
	pipeline_cleanup(&data, CU_SUCCESS);
	return (rc);
}
