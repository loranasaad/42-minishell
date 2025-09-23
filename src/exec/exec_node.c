/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 23:30:53 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/23 23:45:50 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"

int	exec_run_node(t_ast *node, t_ms *ms)
{
	t_cmdspec	spec;
	int			rc;
	
	if (!node)
		return (0);
	if (node->kind == AST_CMD)
	{
		spec.argv = NULL;
		spec.redirs = NULL;
		if (!build_cmdspec_from_segment(node->start, node->end, &spec))
			return (2);
		ms->last_status = exec_one_cmd(&spec, ms);
		free_cmdspec(&spec);
		return (ms->last_status);
	}
	else if (node->kind == AST_PIPE)
	{
		rc = exec_pipeline(node, ms);
		return (rc);
	}
	return (0);
}