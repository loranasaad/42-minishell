/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:10:31 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/29 15:50:28 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include <stdlib.h>

void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	ast_free(node->left);
	ast_free(node->right);
	free(node);
}

void	free_redirs(t_redir **redirs)
{
	t_redir	*w;
	t_redir	*destroy;

	if (!redirs)
		return ;
	w = *redirs;
	while (w)
	{
		destroy = w;
		w = w->next;
		if (destroy->kind == R_HDOC && destroy->hdoc_fd >= 0)
			close(destroy->hdoc_fd);
		free(destroy->target);
		free(destroy);
	}
	*redirs = NULL;
}

void	free_cmdspec(t_cmdspec *spec)
{
	free_str_arr(&(spec->argv));
	free_redirs(&(spec->redirs));
	return ;
}
