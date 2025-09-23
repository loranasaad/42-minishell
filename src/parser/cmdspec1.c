/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:44:52 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 13:02:10 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int		handle_word(t_cmdspec *out, t_token *tok);
static int		handle_redir(t_cmdspec *out, t_token *tok);
static int		is_redir(t_token *tok);
static void		redir_push_back(t_redir **redirs, t_redir *redir);

int	build_cmdspec_from_segment(t_token *start, t_token *end, t_cmdspec *out)
{
	t_token	*w;

	out->argv = NULL;
	out->redirs = NULL;
	w = start;
	while (w != end)
	{
		if (w->kind == TK_WORD)
		{
			if (!handle_word(out, w))
				return (0);
			w = w->next;
		}
		else if (is_redir(w->kind))
		{
			if (!handle_redir(out, w))
				return (0);
			w = w->next->next;
		}
	}
	return (1);
}

static int	handle_word(t_cmdspec *out, t_token *tok)
{
	out->argv = strv_push(out->argv, tok->val);
	if (out->argv == NULL)
	{
		free_cmdspec(out);
		return (0);
	}
	return (1);
}

static int	handle_redir(t_cmdspec *out, t_token *tok)
{
	t_redir	*redir;

	redir = build_tredir(tok);
	if (!redir)
	{
		free_cmdspec(out);
		return (0);
	}
	redir_push_back(&(out->redirs), redir);
	return (1);
}

// return 1 if token kind is TK_IN, TK_OUT, TK_APP or TK_HDOC, else 0
static int	is_redir(t_token *tok)
{
	return (tok->kind == TK_IN
		|| tok->kind == TK_OUT
		|| tok->kind == TK_APP
		|| tok->kind == TK_HDOC);
}

// add a t_redir struct at end of t_redir list
static void	redir_push_back(t_redir **redirs, t_redir *redir)
{
	t_redir	*w;

	if (!redirs || !redir)
		return ;
	if (!*redirs)
	{
		*redirs = redir;
		return ;
	}
	w = *redirs;
	while (w->next)
		w = w->next;
	w->next = redir;
	return ;
}
