/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:44:18 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:59:29 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	read_sq_segment(int *i, const char *line, int *len, t_seen *seen)
{
	int	j;

	j = *i + 1;
	while (line[j] && line[j] != '\'')
		j++;
	if (line[j] == '\0')
		return (-1);
	*len += j - (*i + 1);
	*i = j + 1;
	seen->single_quote = 1;
	return (1);
}

static	int	read_dq_segment(int *i, const char *line, int *len, t_seen *seen)
{
	int	j;

	j = *i + 1;
	while (line[j] && line[j] != '"')
		j++;
	if (line[j] == '\0')
		return (-1);
	*len += j - (*i + 1);
	*i = j + 1;
	seen->double_quote = 1;
	return (1);
}

static	int	read_unquoted_segment(int *i, const char *line, int *len)
{
	int	j;
	int	start_i;

	start_i = *i;
	j = *i;
	while (line[j] && line[j] != '"' && line[j] != '\''
		&& !is_meta(line[j]) && !is_space(line[j]))
	{
		j++;
		(*len)++;
	}
	*i = j;
	if (j > start_i)
		return (1);
	return (0);
}

static	int	process_segment(int *i, const char *line, int *len, t_seen *seen)
{
	if (line[*i] == '\'')
		return (read_sq_segment(i, line, len, seen));
	else if (line[*i] == '"')
		return (read_dq_segment(i, line, len, seen));
	else
		return (read_unquoted_segment(i, line, len));
}

int	read_word(int *i, const char *line, int *quoted)
{
	int		segments;
	t_seen	seen;
	int		len;
	int		seg_result;

	seen.single_quote = 0;
	seen.double_quote = 0;
	segments = 0;
	len = 0;
	while (line[*i] && !is_space(line[*i]) && !is_meta(line[*i]))
	{
		seg_result = process_segment(i, line, &len, &seen);
		if (seg_result < 0)
			return (-1);
		if (seg_result > 0)
			segments++;
	}
	set_quote_type(segments, &seen, quoted);
	return (len);
}
