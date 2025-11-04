/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:59:03 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/29 15:24:59 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "parser.h"
#include <sys/stat.h>

static	void	exec_error_is_dir(char *path, char **envp, t_ms *ms, t_cu *cu)
{
	write(2, "minishell: ", 11);
	write(2, path, ft_strlen(path));
	write(2, ": Is a directory\n", 18);
	free_str_arr(&envp);
	child_cleanup_all(ms, cu);
	exit(126);
}

static	void	exec_err_exit(char *path, char **envp, t_ms *ms, t_cu *cleanup)
{
	if (errno == ENOENT)
	{
		ms_perror("minishell", path);
		free_str_arr(&envp);
		child_cleanup_all(ms, cleanup);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ms_perror("minishell", path);
		free_str_arr(&envp);
		child_cleanup_all(ms, cleanup);
		exit(126);
	}
	ms_perror("minishell", path);
	free_str_arr(&envp);
	child_cleanup_all(ms, cleanup);
	exit(126);
}

void	exec_direct_path(t_cmdspec *spec, char **envp, t_ms *ms, t_cu *cleanup)
{
	struct stat	path_stat;

	if (stat(spec->argv[0], &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			exec_error_is_dir(spec->argv[0], envp, ms, cleanup);
	}
	execve(spec->argv[0], spec->argv, envp);
	exec_err_exit(spec->argv[0], envp, ms, cleanup);
}

void	exec_path_search(t_cmdspec *spec, char **envp, t_ms *ms, t_cu *cleanup)
{
	char	*full_path;

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
