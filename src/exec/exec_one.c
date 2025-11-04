/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/03 21:47:31 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

static	int	handle_hdoc_failure(t_ms *ms)
{
	if (ms->last_status != 0)
		return (ms->last_status);
	else
		return (1);
}

static	int	wait_for_child(pid_t pid, t_redir *redirs)
{
	int	status;
	int	rc;

	init_exec_signals();
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
		{
			ms_perror("minishell", "waitpid");
			hdoc_cleanup(redirs);
			return (1);
		}
	}
	init_prompt_signals();
	hdoc_cleanup(redirs);
	rc = status_to_rc(status);
	return (rc);
}

int	exec_one_cmd(t_cmdspec *spec, t_ms *ms, t_cu *cleanup)
{
	pid_t			pid;
	int				rc;

	if ((!spec->argv || !spec->argv[0]) && !spec->redirs)
		return (0);
	if (!hdoc_prepare(spec->redirs, ms))
		return (handle_hdoc_failure(ms));
	if (spec->argv && spec->argv[0] && is_stateful(spec->argv[0]))
	{
		rc = exec_stateful(spec, ms);
		hdoc_cleanup(spec->redirs);
		return (rc);
	}
	pid = fork();
	if (pid < 0)
	{
		ms_perror("minishell", "fork");
		hdoc_cleanup(spec->redirs);
		return (1);
	}
	if (pid == 0)
		exec_child_command(spec, ms, cleanup);
	rc = wait_for_child(pid, spec->redirs);
	return (rc);
}
