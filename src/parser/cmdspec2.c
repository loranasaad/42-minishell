/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:55:04 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 19:32:24 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void		free_redirs(t_redir **redirs);

void	free_cmdspec(t_cmdspec *spec)
{
	free_str_arr(&(spec->argv));
	free_redirs(&(spec->redirs));
	return ;
}

static void	free_redirs(t_redir **redirs)
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
		free(destroy->target);
		free(destroy);
	}
	*redirs = NULL;
}

// build a t_redir struct
t_redir	*build_tredir(t_token *tok)
{
	t_redir	*redir;

	if (!tok || !tok->next || !tok->next->val)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->kind = tok->kind - TK_IN;
	redir->target = ft_strdup(tok->next->val);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}
