/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 10:01:04 by marvin            #+#    #+#             */
/*   Updated: 2025/05/26 13:30:25 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	countwords(char const *s, char c)
{
	size_t	nwords;
	int		inword;

	nwords = 0;
	inword = 0;
	while (*s)
	{
		if (!inword && *s != c)
		{
			nwords++;
			inword = 1;
		}
		else if (inword && *s == c)
			inword = 0;
		s++;
	}
	return (nwords);
}

char	*allocate_word(char const *s, size_t begin, size_t end)
{
	char	*word;

	word = malloc(end - begin + 1);
	if (!word)
	{
		return (NULL);
	}
	ft_memcpy(word, s + begin, end - begin);
	word[end - begin] = '\0';
	return (word);
}

void	find_and_allocate_words(char const *s, char c, char **w, size_t w_i)
{
	size_t	wordbegin;
	size_t	i;
	size_t	slen;
	int		inword;

	wordbegin = 0;
	inword = 0;
	i = 0;
	slen = ft_strlen(s);
	while (i <= slen)
	{
		if (!inword && s[i] != c)
		{
			wordbegin = i;
			inword = 1;
		}
		else if (inword && (s[i] == c || s[i] == '\0'))
		{
			inword = 0;
			w[w_i] = allocate_word(s, wordbegin, i);
			w_i++;
		}
		i++;
	}
}

void	free_words(char **words, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(words[i]);
		i++;
	}
	free(words);
}

char	**ft_split(char const *s, char c)
{
	char	**strings_array;
	size_t	nwords;
	size_t	i;

	i = 0;
	nwords = countwords(s, c);
	strings_array = malloc(sizeof(char *) * (nwords + 1));
	if (!strings_array)
		return (NULL);
	find_and_allocate_words(s, c, strings_array, 0);
	while (i < nwords)
	{
		if (!strings_array[i])
		{
			free_words(strings_array, nwords);
			return (NULL);
		}
		i++;
	}
	strings_array[nwords] = NULL;
	return (strings_array);
}
