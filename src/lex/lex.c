/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:42:27 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 14:17:13 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	read_word(int *i, const char *line, int *quoted)
{
	int	segments;
	int	sq_seen;
	int	dq_seen;
	int	len;
	int	j;
	
	sq_seen = 0;
	dq_seen = 0;
	segments = 0;
	len = 0;
	while(line[*i] && !is_space(line[*i]) && !is_meta(line[*i]))
	{
		if (line[*i] == '\'')
		{
			j = *i + 1;
			while (line[j] && line[j] != '\'')
				j++;
			if (line[j] == '\0')
				return (-1);
			len += j - (*i + 1);
			*i = j + 1;
			sq_seen = 1;
			segments++;
			continue ;
		}
		else if (line[*i] == '"')
		{
			j = *i + 1;
			while (line[j] && line[j] != '"')
				j++;
			if (line[j] == '\0')
				return (-1);
			len += j - (*i + 1);
			*i = j + 1;
			dq_seen = 1;
			segments++;
			continue ;
		}
		else
		{
			j = *i;
			while (line[j] && line[j] != '"' && line[j] != '\'' 
					&& !is_meta(line[j]) && !is_space(line[j]))
			{
				j++;
				len++;
			}
			if (j > *i)
				segments++;
			*i = j;
			continue ;
		}
	}
	if (segments == 1 && sq_seen == 1 && dq_seen == 0)
		*quoted = 1;
	else if (segments == 1 && sq_seen == 0 && dq_seen == 1)
		*quoted = 2;
	else
		*quoted = 0;
	return (len);
}

static char	*copy_word(int *k, const char *line, int *quoted)
{
	int		len;
	char	*str;
	int		i;
	int		j;
	int		l;
	
	i = *k;
	len = read_word(k, line, quoted);
	if (len == -1)
		return (NULL);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	l = 0;
	while(line[i] && i < *k)
	{
		if (line[i] == '\'')
		{
			j = i + 1;
			while (j < *k && line[j] != '\'')
			{	
				str[l] = line[j];
				j++;
				l++;
			}
			i = j + 1;
			continue ;
		}
		else if (line[i] == '"')
		{
			j = i + 1;
			while (j < *k && line[j] != '"')
			{	
				str[l] = line[j];
				j++;
				l++;
			}
			i = j + 1;
			continue ;
		}
		else
		{
			j = i;
			while (j < *k && line[j] != '"' && line[j] != '\'' 
					&& !is_meta(line[j]) && !is_space(line[j]))
			{	
				str[l] = line[j];
				j++;
				l++;
			}
			i = j;
			continue ;
		}
	}
	str[l] = '\0';
	return (str);
}

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
	if(!str)
	{
		free_tokens(*head);
		return (-1);
	}
	tok_push_back(head, tok_new(TK_WORD, str, quoted));
	return (0);
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
			push_meta(&i, line, &head, line[i]);
			continue ;
		}
		else
		{
			if(push_word(&i, line, &head) < 0)
			{
				*lex_status = 2;
				return (NULL);
			}
			continue;
		}
	}
	return (head);
}
