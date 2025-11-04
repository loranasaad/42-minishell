/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:44:52 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 15:56:11 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// return 1 if token kind is TK_IN, TK_OUT, TK_APP or TK_HDOC, else 0
static int	is_redir(t_token *tok)
{
	return (tok->kind == TK_IN
		|| tok->kind == TK_OUT
		|| tok->kind == TK_APP
		|| tok->kind == TK_HDOC);
}

static int	handle_word(t_cmdspec *out, t_token *tok, t_ms *ms)
{
	int		exp_status;
	int		split_status;
	char	*val_exp;

	val_exp = handle_var_expansion(tok->val, tok->quoted, ms, &exp_status);
	if (!exp_status)
	{
		free_cmdspec(out);
		return (0);
	}
	free(tok->val);
	tok->val = val_exp;
	split_status = 1;
	if (several_fields(tok->quoted, tok->val))
		split_status = field_split(tok->val, out);
	else
		out->argv = strv_push(out->argv, tok->val);
	if (out->argv == NULL || split_status == 0)
	{
		free_cmdspec(out);
		return (0);
	}
	return (1);
}

static void	handle_empty_arg(t_cmdspec *out)
{
	int	i;
	int	j;

	if (!out || !out->argv)
		return ;
	i = 0;
	j = 0;
	while (out->argv[i] && out->argv[i][0] == '\0')
	{
		free(out->argv[i]);
		i++;
	}
	while (out->argv[i])
		out->argv[j++] = out->argv[i++];
	out->argv[j] = NULL;
}

int	build_cmdspec_from_segment(t_token *start,
	t_token *end,
	t_cmdspec *out,
	t_ms *ms)
{
	t_token	*w;

	out->argv = NULL;
	out->redirs = NULL;
	w = start;
	while (w != end)
	{
		if (w->kind == TK_WORD)
		{
			if (!handle_word(out, w, ms))
				return (0);
			w = w->next;
		}
		else if (is_redir(w))
		{
			if (!handle_redir(out, w, ms))
				return (0);
			w = w->next->next;
		}
	}
	handle_empty_arg(out);
	return (1);
}
