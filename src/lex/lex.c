/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:42:27 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:42:24 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	push_meta(int *i, const char *line, t_token **head, char c)
{
	if (c != '|' && match_2(line, *i, c, c))
	{
		if (c == '<')
			tok_push_back(head, tok_new(TK_HDOC, NULL, 0));
		else
			tok_push_back(head, tok_new(TK_APP, NULL, 0));
		*i += 2;
	}
	else
	{
		if (c == '<')
			tok_push_back(head, tok_new(TK_IN, NULL, 0));
		else if (c == '>')
			tok_push_back(head, tok_new(TK_OUT, NULL, 0));
		else
			tok_push_back(head, tok_new(TK_PIPE, NULL, 0));
		(*i)++;
	}
}

static int	push_word(int *i, const char *line, t_token **head)
{
	char	*str;
	int		quoted;

	str = copy_word(i, line, &quoted);
	if (!str)
	{
		free_tokens(*head);
		return (-1);
	}
	tok_push_back(head, tok_new(TK_WORD, str, quoted));
	return (0);
}

static	int	process_token(int *i, const char *line, t_token **head)
{
	if (is_space(line[*i]))
	{
		(*i)++;
		return (0);
	}
	if (is_meta(line[*i]))
	{
		push_meta(i, line, head, line[*i]);
		return (0);
	}
	else
		return (push_word(i, line, head));
}

t_token	*lex_line(const char *line, int *lex_status)
{
	int		i;
	t_token	*head;

	head = NULL;
	*lex_status = 0;
	i = 0;
	while (line[i])
	{
		if (process_token(&i, line, &head) < 0)
		{
			*lex_status = 2;
			return (NULL);
		}
	}
	return (head);
}
