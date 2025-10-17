/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/17 17:07:45 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int	exec_one_cmd(const t_cmdspec *spec, t_ms *ms)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*full_path;
	int		rc;

	if ((!spec->argv || !spec->argv[0]) && !spec->redirs)
		return (0);
	pid = fork();
	if (pid < 0)
	{
		ms_perror("minishell", "fork");
		return (1);
	}	
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if(!apply_redirs(spec->redirs))
			exit(1);
		if(!spec->argv || !spec->argv[0])		//filter the pure redirections 
			exit(0);
		if (is_builtin(spec->argv[0]))
		{
			builtin_dispatch(spec->argv, ms, &rc);
			free_cmdspec(&spec);
			exit (rc);
		}
		envp = env_to_envp(ms->env);
		if (!envp)
		{
			ms_perror("minishell", "env");
			exit(1);
		}
		//if it has a direct path
		if (ft_strchr(spec->argv[0], '/'))
		{
			execve(spec->argv[0], spec->argv, envp);
			if (errno == ENOENT)
				exit(127);
			exit(126);			
		}
		full_path = find_in_path(spec->argv[0], ms->env);
		if (!full_path)
		{
			exec_error(spec->argv[0]);
			exit(127);
		}
		execve(full_path, spec->argv, envp);
		if (errno == ENOENT)
			exit(127);
		exit(126);		
	}	
	if (waitpid(pid, &status, 0) < 0)
	{
		ms_perror("minishell", "waitpid");
		return (1);
	}
	return (status_to_rc(status));
}
