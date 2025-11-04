/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:13:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:41:48 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_quote_type(int seg, t_seen *seen, int *quoted)
{
	if (seg == 1 && seen->single_quote == 1 && seen->double_quote == 0)
		*quoted = 1;
	else if (seg == 1 && seen->single_quote == 0 && seen->double_quote == 1)
		*quoted = 2;
	else if (seen->single_quote || seen->double_quote)
		*quoted = 3;
	else
		*quoted = 0;
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_meta(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	match_2(const char *s, int i, char a, char b)
{
	if (s[i] && s[i + 1] && s[i] == a && s[i + 1] == b)
		return (1);
	return (0);
}
