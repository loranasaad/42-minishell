/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_setup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:01:53 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:13:17 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"

int	prepare_all_heredocs(t_cmdspec *specs, int len, t_ms *ms)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (!hdoc_prepare(specs[i].redirs, ms))
		{
			if (ms->last_status)
				return (ms->last_status);
			return (1);
		}
		i++;
	}
	return (0);
}

int	build_all_specs(t_ast **stages, int len, t_cmdspec *specs, t_ms *ms)
{
	int	i;

	i = 0;
	while (i < len)
	{
		specs[i].argv = NULL;
		specs[i].redirs = NULL;
		if (!build_cmdspec_from_segment(stages[i]->start,
				stages[i]->end, &specs[i], ms))
		{
			free_specs(specs, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	make_pipes(int (*pipes)[2], int len)
{
	int	i;

	i = 0;
	while (i < len - 1)
	{
		if (pipe(pipes[i]) < 0)
		{
			ms_perror("minishell", "pipe");
			close_pipes(pipes, i + 1);
			return (0);
		}
		i++;
	}
	return (1);
}

int	count_pipeline(t_ast *root)
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

void	flatten_pipeline(t_ast *root, t_ast **commands, int length)
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
