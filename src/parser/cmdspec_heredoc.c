/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:36:31 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 15:41:09 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_redir	*build_heredoc(t_token *tok)
{
	t_redir	*redir;

	if (!tok || !tok->next || !tok->next->val)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->kind = R_HDOC;
	redir->hdoc_fd = -1;
	redir->target = ft_strdup(tok->next->val);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	if (tok->next->quoted == 0)
		redir->hdoc_exp = 1;
	else
		redir->hdoc_exp = 0;
	redir->next = NULL;
	return (redir);
}
