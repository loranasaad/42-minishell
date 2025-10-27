/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:47 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/27 23:36:54 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

static	void	hdoc_cleanup_specs(t_cmdspec *s, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		hdoc_cleanup(s[i].redirs);
		i++;
	}
}

static	void	free_specs(t_cmdspec *specs, int len)
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

static	void	close_pipes(int (* pipes)[2], int len)
{
	int	i;
	
	if(!pipes)
		return;
	i = 0;
	while(i < len - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static	int	make_pipes(int (* pipes)[2], int len)
{
	int	i;
	
	i = 0;
	while (i < len - 1)	
	{
		if(pipe(pipes[i]) < 0)
		{
			ms_perror("minishell", "pipe");
			close_pipes(pipes, i + 1);
			return (0);
		}
		i++;
	}
	return (1);
}

static	int	count_pipeline(t_ast *root)
{
	int		count;
	t_ast	*current;

	if (!root)
		return (0);
	current = root;
	count = 0;
	while (current && current->kind == AST_PIPE)
	{
		count++;
		current = current->left;
	}
	count++;
	return (count);
}

static	void	flatten_pipeline(t_ast *root, t_ast **commands, int	length)
{
	int		i;
	t_ast	*current;

	i = 0;
	current = root;
	while (current && current->kind == AST_PIPE)
	{
		commands[length - 1 - i] = current->right;
		current = current->left;
		i++;
	}
	commands[0] = current;
}
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
				break;
		}
		if (i == len - 1)
			rc = status_to_rc(status);
		i++;
	}
	return (rc);
}

static	void exec_child(int i, int len, int (* pipes)[2], t_cmdspec *spec, t_ms *ms, t_cu *cleanup)
{
	char	**envp;
	char	*full_path;
	int		rc;
	
	rc = 0;
	signal(SIGINT, SIG_DFL);	//handle signals
	signal(SIGQUIT, SIG_DFL);
	if (i > 0 && dup2(pipes[i - 1][0], STDIN_FILENO) < 0)	//apply and handle error with dup2
	{
		ms_perror("minishell", "dup2");
		child_cleanup_all(ms, cleanup);
		exit (1);
	}
	if (i < len - 1 && dup2(pipes[i][1], STDOUT_FILENO) < 0)
	{
		ms_perror("minishell", "dup2");
		child_cleanup_all(ms, cleanup);
		exit (1);
	}
	close_pipes(pipes, len);
	if (!apply_redirs(spec->redirs))
	{
		child_cleanup_all(ms, cleanup);
		exit (1);
	}
	if(!spec->argv || !spec->argv[0])		//filter the pure redirections 
	{
		child_cleanup_all(ms, cleanup);
		exit (0);
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
	if (ft_strchr(spec->argv[0], '/')) // fix_laura beginn
	{
		if (is_a_dir(spec->argv[0]))
		{
			print_dir_err_msg(spec->argv[0]);
			free_str_arr(&envp);
			child_cleanup_all(ms, cleanup);
			exit (126);
		}
		execve(spec->argv[0], spec->argv, envp);
		int saved_errno = errno;
		int	exit_status = 0;
		if (saved_errno == ENOENT)
			exit_status = 127;
		else if (saved_errno == EACCES)
			exit_status = 126;
		else
			exit_status = 1;
		print_general_err_msg(spec->argv[0], saved_errno);
		free_str_arr(&envp);
		child_cleanup_all(ms, cleanup);
		exit(exit_status);
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
	int saved_errno = errno;
	print_general_err_msg(spec->argv[0], saved_errno);
	if (saved_errno == ENOENT)
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

int	exec_pipeline(t_ast *root, t_ms *ms, t_cu *cleanup)
{
	t_ast		**stages;
	int			len;
	pid_t		*pids;
	int			(*pipes)[2];
	int			i;
	int			rc;
	t_cmdspec	*specs;

	len = count_pipeline(root);
	stages = malloc(len * sizeof(*stages));
	if (!stages)
		return (1);
	flatten_pipeline(root, stages, len);
	specs = malloc(sizeof(*specs) * len);
	if (!specs)
	{
		free(stages);
		return (1);
	}
	i = 0;
	while (i < len)
	{
		specs[i].argv = NULL;
		specs[i].redirs = NULL;
		if (!build_cmdspec_from_segment(stages[i]->start, stages[i]->end, &specs[i], ms))
		{
			free_specs(specs, i);
			free(stages);
			return(2);
		}
		i++;
	}
	i = 0;
	while (i < len)
	{
		if (!hdoc_prepare(specs[i].redirs, ms))		//hdoc preparation
		{
			hdoc_cleanup_specs(specs, len);
			free_specs(specs, len);
			free(stages);
			if (ms->last_status)
				return (ms->last_status);
			return (1);
		}
		i++;
	}
	pipes = malloc(sizeof(*pipes) * (len - 1));
	if (!pipes)
	{
		hdoc_cleanup_specs(specs, len);
		free_specs(specs, len);
		free(stages);
		return (1);
	}
	if(!make_pipes(pipes, len))
	{
		free(pipes);
		hdoc_cleanup_specs(specs, len);
		free_specs(specs, len);
		free(stages);
		return (1);
	}
	pids = malloc(sizeof(pid_t) * len);
	if (!pids)
	{
		close_pipes(pipes, len);
		free(pipes);
		hdoc_cleanup_specs(specs, len);
		free_specs(specs, len);
		free(stages);
		return (1);
	}
	i = 0;
	while (i < len)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{	
			ms_perror("minishell", "fork");
			close_pipes(pipes, len);
			free(pipes);
			hdoc_cleanup_specs(specs, len);
			free_specs(specs, len);
			free(stages);
			free(pids);
			return (1);
		}
		else if (pids[i] == 0)		//child
		{
			cleanup->spec = &specs[i];
			cleanup->stages = stages;
			cleanup->specs = specs;
			cleanup->pipes = pipes;
			cleanup->pids = pids;
			cleanup->pipe_len = len;
			exec_child(i, len, pipes, &specs[i], ms, cleanup);
		}
		i++;
	}
	close_pipes(pipes, len);
	init_exec_signals();
	rc = wait_pipeline(pids, len);
	init_prompt_signals();
	free(pipes);
	hdoc_cleanup_specs(specs, len);
	free(pids);
	free_specs(specs, len);
	free(stages);
	return (rc);
}
