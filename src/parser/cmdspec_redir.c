/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 15:36:34 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 15:59:58 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	redir_several_fields_error(const char *val)
{
	const char	*s1;
	const char	*s2;

	s1 = "minishell: ";
	s2 = ": ambiguous redirect\n";
	write(2, s1, ft_strlen(s1));
	write(2, val, ft_strlen(val));
	write(2, s2, ft_strlen(s2));
}

static t_redir	*build_tredir(t_token *tok, t_ms *ms)
{
	t_redir	*redir;
	int		status;
	int		multiple;

	if (!tok || !tok->next || !tok->next->val)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->kind = tok->kind - TK_IN;
	redir->target = handle_var_expansion(tok->next->val,
			tok->next->quoted, ms, &status);
	multiple = several_fields(tok->next->quoted, redir->target);
	if (!redir->target || !status || multiple)
	{
		if (multiple)
			redir_several_fields_error(tok->next->val);
		free(redir->target);
		free(redir);
		return (NULL);
	}
	redir->hdoc_exp = -1;
	redir->hdoc_fd = -1;
	redir->next = NULL;
	return (redir);
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

int	handle_redir(t_cmdspec *out, t_token *tok, t_ms *ms)
{
	t_redir	*redir;

	if (tok->kind == TK_HDOC)
		redir = build_heredoc(tok);
	else
		redir = build_tredir(tok, ms);
	if (!redir)
	{
		free_cmdspec(out);
		return (0);
	}
	redir_push_back(&(out->redirs), redir);
	return (1);
}
