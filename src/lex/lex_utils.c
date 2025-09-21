/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:13:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 13:13:15 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	word_len(char *str, int *i)
{
	int len;
	
	len = 0;
	while (!is_meta(str[*i]) && !is_space(str[*i]))
	{
		*i += len++;
	}
	return (len);
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