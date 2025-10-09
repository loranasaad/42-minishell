/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:55:04 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/06 19:51:42 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void		free_redirs(t_redir **redirs);
static void		redir_several_fields_error(const char *val);

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
t_redir	*build_tredir(t_token *tok, t_ms *ms)
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
	redir->target = handle_var_expansion(tok->next->val, tok->next->quoted, ms, &status);
	multiple = several_fields(tok->next->quoted, redir->target);
	if (!redir->target || !status || multiple)
	{
		if (multiple)
			redir_several_fields_error(tok->next->val);
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static void	redir_several_fields_error(const char *val)
{
	const	char	*s1;
	const	char	*s2;

	s1 = "minishell: ";
	s2 = ": ambiguous redirect\n";

	write(2, s1, ft_strlen(s1));
	write(2, val, ft_strlen(val));
	write(2, s2, ft_strlen(s2));
}
