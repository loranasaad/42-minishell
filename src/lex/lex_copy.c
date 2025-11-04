/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:08:14 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:22:30 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	copy_sq_segment(const char *line, int *i, char *str, int *l)
{
	int	j;

	j = *i + 1;
	while (line[j] && line[j] != '\'')
	{
		str[*l] = line[j];
		j++;
		(*l)++;
	}
	*i = j + 1;
}

static	void	copy_dq_segment(const char *line, int *i, char *str, int *l)
{
	int	j;

	j = *i + 1;
	while (line[j] && line[j] != '"')
	{
		str[*l] = line[j];
		j++;
		(*l)++;
	}
	*i = j + 1;
}

static	void	copy_unquoted_seg(const char *line, int *i, char *str, int *l)
{
	int	j;

	j = *i;
	while (line[j] && line[j] != '"' && line[j] != '\''
		&& !is_meta(line[j]) && !is_space(line[j]))
	{
		str[*l] = line[j];
		j++;
		(*l)++;
	}
	*i = j;
}

static	void	copy_segments(const char *line, int *i, int k, char *str)
{
	int	l;

	l = 0;
	while (line[*i] && *i < k)
	{
		if (line[*i] == '\'')
			copy_sq_segment(line, i, str, &l);
		else if (line[*i] == '"')
			copy_dq_segment(line, i, str, &l);
		else
			copy_unquoted_seg(line, i, str, &l);
	}
	str[l] = '\0';
}

char	*copy_word(int *k, const char *line, int *quoted)
{
	int		len;
	char	*str;
	int		i;

	i = *k;
	len = read_word(k, line, quoted);
	if (len == -1)
		return (NULL);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	copy_segments(line, &i, *k, str);
	return (str);
}
