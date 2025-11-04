/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:53:22 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:54:18 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static	int	is_word(const t_token *t)
{
	if (t && t->kind == TK_WORD)
		return (1);
	return (0);
}

static	int	is_redir(const t_token *t)
{
	if (t && (t->kind == TK_APP || t->kind == TK_HDOC || t->kind == TK_IN
			|| t->kind == TK_OUT))
		return (1);
	return (0);
}

static	int	is_pipe(const t_token *t)
{
	if (t && t->kind == TK_PIPE)
		return (1);
	return (0);
}

// ensure there is a word after a pipe/redir
int	parser_validate(t_token *token)
{
	t_token	*t;

	t = token;
	if (!t)
		return (1);
	if (is_pipe(t))
		return (0);
	while (t)
	{
		if (is_pipe(t))
		{
			if (is_pipe(t->next) || !(t->next))
				return (0);
		}
		if (is_redir(t))
		{
			if (!is_word(t->next) || !(t->next))
				return (0);
		}
		t = t->next;
	}
	return (1);
}
