/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:48:14 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:15:37 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/wait.h>
#include "exec.h"

void	child_cleanup_all(t_ms *ms, t_cu *cleanup)
{
	int	i;

	if (cleanup->spec)
		free_cmdspec(cleanup->spec);
	if (cleanup->ast)
		ast_free(cleanup->ast);
	if (cleanup->toks)
		free_tokens(cleanup->toks);
	if (cleanup->specs && cleanup->pipe_len > 0)
	{
		i = 0;
		while (i < cleanup->pipe_len)
		{
			free_cmdspec(&cleanup->specs[i]);
			i++;
		}
		free(cleanup->specs);
	}
	if (cleanup->stages)
		free(cleanup->stages);
	if (cleanup->pipes)
		free(cleanup->pipes);
	if (cleanup->pids)
		free(cleanup->pids);
	env_free(&(ms->env));
}

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "echo") == 0)
		return (1);
	if (ft_strcmp(name, "pwd") == 0)
		return (1);
	if (ft_strcmp(name, "env") == 0)
		return (1);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	return (0);
}

int	is_stateful(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "cd") == 0)
		return (1);
	if (ft_strcmp(name, "export") == 0)
		return (1);
	if (ft_strcmp(name, "unset") == 0)
		return (1);
	if (ft_strcmp(name, "exit") == 0)
		return (1);
	return (0);
}

void	exec_error(const char *cmd)
{
	const char	*s1;
	const char	*s2;

	s1 = "minishell: ";
	s2 = ": command not found\n";
	write(2, s1, ft_strlen(s1));
	write(2, cmd, ft_strlen(cmd));
	write(2, s2, ft_strlen(s2));
}

int	status_to_rc(int st)
{
	if (WIFEXITED(st))
		return (WEXITSTATUS(st));
	if (WIFSIGNALED(st))
		return (128 + WTERMSIG(st));
	return (1);
}
