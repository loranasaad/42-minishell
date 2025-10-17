/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you                                            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24                                    #+#    #+#         */
/*   Updated: 2025/09/24                                    ###   ########.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "exec.h"
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* ---------- small helpers ------------------------------------------------ */

/* Count number of commands in a left-nested PIPE tree:
      PIPE
     /    \
  PIPE     C    -> result = 3 (A,B,C)
 /    \
A      B
*/
static int	count_pipeline(const t_ast *root)
{
	int n;

	n = 0;
	while (root && root->kind == AST_PIPE)
	{
		n++;
		root = root->left;
	}
	if (root)
		n++;
	return (n);
}

/* Flatten the tree into stages[0..n-1] as left→right command order. */
static void	flatten_pipeline(const t_ast *root, t_ast **stages, int n)
{
	int   i;
	const t_ast *cur;

	i = 0;
	cur = root;
	while (cur && cur->kind == AST_PIPE)
	{
		stages[n - 1 - i] = (t_ast *)cur->right;
		cur = cur->left;
		i++;
	}
	stages[0] = (t_ast *)cur;
}

/* Create N-1 pipes (pair array size = n-1). Return 1 ok / 0 err. */
static int	make_pipes(int (*pipes)[2], int n)
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) < 0)
		{
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

/* Close all pipe fds in this array. */
static void	close_all_pipes(int (*pipes)[2], int n)
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/* Duplicate pipe ends for child i (0..n-1), then close all pipe fds. */
static int	setup_child_fds(int i, int n, int (*pipes)[2])
{
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
			return (0);
	}
	if (i < n - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
			return (0);
	}
	close_all_pipes(pipes, n);
	return (1);
}

/* Run one stage in the CHILD:
   - build cmdspec from the stage node
   - set default signals
   - apply redirs
   - resolve + exec (same policy as exec_one child path)
   This function NEVER returns. It exits the child.
*/
static void	run_stage_child(t_ast *cmd_node, t_ms *ms)
{
	t_cmdspec	spec;
	char		**envp;
	char		*full;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

	spec.argv = NULL;
	spec.redirs = NULL;
	if (!build_cmdspec_from_segment(cmd_node->start, cmd_node->end, &spec))
		_exit(2);

	/* Pure redirs in a pipeline still run as a process that just sets fds. */
	if (!apply_redirs(spec.redirs))
	{
		free_cmdspec(&spec);
		_exit(1);
	}
	if (!spec.argv || !spec.argv[0])
	{
		free_cmdspec(&spec);
		_exit(0);
	}

	/* Builtins-in-pipeline: later you may dispatch here; for now treat as extern. */

	envp = env_to_envp(ms->env);
	if (!envp)
	{
		free_cmdspec(&spec);
		ms_perror("minishell", "env");
		_exit(1);
	}

	/* Direct path? try execve(argv[0]) */
	if (ft_strchr(spec.argv[0], '/'))
	{
		execve(spec.argv[0], spec.argv, envp);
		free_envp(envp);
		free_cmdspec(&spec);
		if (errno == ENOENT)
			_exit(127);
		_exit(126);
	}

	/* PATH lookup */
	full = find_in_path(spec.argv[0], ms->env);
	if (!full)
	{
		free_envp(envp);
		free_cmdspec(&spec);
		exec_error(spec.argv[0]);
		_exit(127);
	}
	execve(full, spec.argv, envp);

	/* If we’re here, execve failed. */
	free(full);
	free_envp(envp);
	free_cmdspec(&spec);
	if (errno == ENOENT)
		_exit(127);
	_exit(126);
}

/* ---------- main entry --------------------------------------------------- */

int	exec_pipeline(t_ast *root, t_ms *ms)
{
	int		n;
	t_ast	**stages;
	int		(*pipes)[2];
	pid_t	*pids;
	int		i;
	int		rc;

	n = count_pipeline(root);
	if (n <= 0)
		return (2);

	stages = (t_ast **)malloc(sizeof(t_ast *) * n);
	if (!stages)
		return (1);
	flatten_pipeline(root, stages, n);

	/* Single stage should normally not be routed here, but handle gracefully. */
	if (n == 1)
	{
		t_cmdspec spec;

		spec.argv = NULL;
		spec.redirs = NULL;
		if (!build_cmdspec_from_segment(stages[0]->start, stages[0]->end, &spec))
		{
			free(stages);
			return (2);
		}
		rc = exec_one_cmd(&spec, ms);
		free_cmdspec(&spec);
		free(stages);
		return (rc);
	}

	pipes = (int (*)[2])malloc(sizeof(int[2]) * (n - 1));
	if (!pipes)
	{
		free(stages);
		return (1);
	}
	if (!make_pipes(pipes, n))
	{
		free(pipes);
		free(stages);
		return (1);
	}

	pids = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!pids)
	{
		close_all_pipes(pipes, n);
		free(pipes);
		free(stages);
		return (1);
	}

	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			ms_perror("minishell", "fork");
			/* Best effort: parent closes and waits the ones already forked */
			close_all_pipes(pipes, n);
			rc = wait_pipeline(pids, i); /* wait the few that started */
			free(pids);
			free(pipes);
			free(stages);
			return (1);
		}
		if (pids[i] == 0)
		{
			if (!setup_child_fds(i, n, pipes))
				_exit(1);
			run_stage_child(stages[i], ms); /* never returns */
		}
		i++;
	}

	/* Parent: no longer needs any pipe fds */
	close_all_pipes(pipes, n);

	rc = wait_pipeline(pids, n);

	free(pids);
	free(pipes);
	free(stages);
	return (rc);
}