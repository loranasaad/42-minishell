/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:55:04 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 13:02:01 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static size_t	len_str_arr(char **words);
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

// return length of a string array
static size_t	len_str_arr(char **words)
{
	size_t	i;

	i = 0;
	if (!words)
		return (0);
	while (words[i])
	{
		i++;
	}
	return (i);
}

// build a t_redir struct
static t_redir	*build_tredir(t_token *tok)
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
