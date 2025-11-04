/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:08:29 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void	set_child_pipes(int i, t_pipe_data *data, t_ms *ms, t_cu *c_up)
{
	if (i > 0 && dup2(data->pipes[i - 1][0], STDIN_FILENO) < 0)
	{
		ms_perror("minishell", "dup2");
		child_cleanup_all(ms, c_up);
		exit (1);
	}
	if (i < data->len - 1 && dup2(data->pipes[i][1], STDOUT_FILENO) < 0)
	{
		ms_perror("minishell", "dup2");
		child_cleanup_all(ms, c_up);
		exit (1);
	}
}

static	void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static	void	setup_child_redir(t_cmdspec *spec, t_ms *ms, t_cu *cleanup)
{
	if (!apply_redirs(spec->redirs))
	{
		child_cleanup_all(ms, cleanup);
		exit(1);
	}
	if (!spec->argv || !spec->argv[0])
	{
		child_cleanup_all(ms, cleanup);
		exit(0);
	}
}

void	exec_child_command(t_cmdspec *spec, t_ms *ms, t_cu *cleanup)
{
	char	**envp;
	int		rc;

	setup_child_signals();
	setup_child_redir(spec, ms, cleanup);
	if (is_builtin(spec->argv[0]))
	{
		builtin_dispatch(spec->argv, ms, &rc, 0);
		child_cleanup_all(ms, cleanup);
		exit (rc);
	}
	envp = env_to_envp(ms->env);
	if (!envp)
	{
		ms_perror("minishell", "env");
		child_cleanup_all(ms, cleanup);
		exit(1);
	}
	if (ft_strchr(spec->argv[0], '/'))
		exec_direct_path(spec, envp, ms, cleanup);
	else
		exec_path_search(spec, envp, ms, cleanup);
}
