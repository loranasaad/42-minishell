/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/23 02:12:47 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>


int	exec_one_cmd(const t_cmdspec *spec, t_ms *ms)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*path;
	int		rc;
	
	if (!spec->argv)
		return (0);
	pid = fork();
	if (pid < 0)
		ms_perror("minishell", "fork");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if(!apply_redirs(spec->redirs))
			exit(1);
		if(!spec->argv || !spec->argv[0])
			exit(0);
		//if built in skip for now
		if(notbuiltin)
			{
				execve()
			}
		envp = env_to_envp(ms->env);
		if (!envp)
			exit(1);
		
		
	}	
	
	
	
	
	
	
	
	return (1);
}