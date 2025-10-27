/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/27 15:27:37 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

static	int	save_std(int std_backup[2])
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

static	void	restore_std(int std_backup[2])
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

static	int	exec_stateful(t_cmdspec *spec, t_ms *ms)
{
	int	std_backup[2];
	int	rc;

	if (spec->argv && spec->argv[0] && ft_strcmp(spec->argv[0], "exit") == 0)	//Loran: leaks . this is for exit only
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

int	exec_one_cmd(t_cmdspec *spec, t_ms *ms, t_cu *cleanup)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*full_path;
	int		rc;

	if ((!spec->argv || !spec->argv[0]) && !spec->redirs)
		return (0);
	if (!hdoc_prepare(spec->redirs, ms))		//hdoc preparation
	{
		if (ms->last_status != 0)
			rc = ms->last_status;
		else
			rc = 1;
		return (rc);
	}
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
	if (pid == 0)	//child
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if(!apply_redirs(spec->redirs))
		{
			child_cleanup_all(ms, cleanup);
			exit(1);
		}
		if(!spec->argv || !spec->argv[0])		//filter the pure redirections 
		{
			child_cleanup_all(ms, cleanup);
			exit(0);
		}
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
		//if it has a direct path
		if (ft_strchr(spec->argv[0], '/'))
		{
			execve(spec->argv[0], spec->argv, envp);
			if (errno == ENOENT)
			{
				free_str_arr(&envp);
				child_cleanup_all(ms, cleanup);
				exit(127);
			}
			free_str_arr(&envp);
			child_cleanup_all(ms, cleanup);
			exit(126);
		}
		full_path = find_in_path(spec->argv[0], ms->env);
		if (!full_path)
		{
			exec_error(spec->argv[0]);
			free_str_arr(&envp);
			child_cleanup_all(ms, cleanup);
			exit(127);
		}
		execve(full_path, spec->argv, envp);
		if (errno == ENOENT)
		{
			free(full_path);
			free_str_arr(&envp);
			child_cleanup_all(ms, cleanup);
			exit(127);
		}
		free(full_path);
		free_str_arr(&envp);
		child_cleanup_all(ms, cleanup);
		exit(126);
	}
	init_exec_signals();
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
		{
			ms_perror("minishell", "waitpid");
			hdoc_cleanup(spec->redirs);
			return (1);
		}
	}
	init_prompt_signals();
	hdoc_cleanup(spec->redirs);
	return (status_to_rc(status));
}
