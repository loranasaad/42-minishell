/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:42:27 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/20 19:25:22 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	push_meta(int *i, t_token *head, char *line, char c)
{
	if (c != '!' && match_2(line, i, c, c))
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
			tok_push_back(&head, tok_new(TK_PIPE, NULL, 0));
		*i++;
	}
}

static	void	read_word(int *i, t_token *head, char *line)
{
	
	word_len(line, i);
	
}

t_token	*lex_line(const char *line, int *lex_status)
{
	int	i;
	
	t_token	*head = NULL;
	*lex_status = 0;
	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i++;
			continue;	
		}
		if (is_meta(line[i]))
		{
			push_meta(&i, head, line, line[i]);
			continue ;
		}
		else
			push_word();
	}
	return (&head);
}
