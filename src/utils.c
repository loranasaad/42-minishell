/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:47:55 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/19 13:40:22 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_str_arr(char ***words)
{
	int	i;

	if (!words || !*words)
		return ;
	i = 0;
	while ((*words)[i])
	{
		free((*words)[i]);
		i++;
	}
	free(*words);
	*words = NULL;
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	char	*begin;

	len = ft_strlen(s);
	dup = (char *) malloc(len + 1);
	if (!dup)
		return (NULL);
	begin = dup;
	while (*s)
		*dup++ = *s++;
	*dup = '\0';
	return (begin);
}

size_t	ft_strlen(const char *s)
{
	const char	*begin;

	begin = s;
	while (*s)
		s++;
	return (s - begin);
}

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*result;
	char	*dest;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = (char *) malloc(len);
	if (!result)
		return (NULL);
	dest = result;
	while (*s1)
		*dest++ = *s1++;
	while (*s2)
		*dest++ = *s2++;
	*dest = '\0';
	return (result);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if ((unsigned char) c == *s)
			return ((char *) s);
		s++;
	}
	if ((unsigned char) c == *s)
		return ((char *) s);
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*substr;

	s_len = ft_strlen(s);
	if (len == 0 || start >= s_len)
	{
		substr = (char *) malloc(1);
		if (!substr)
			return (NULL);
		substr[0] = '\0';
		return (substr);
	}
	if (s_len - start < len)
		len = s_len - start;
	substr = (char *) malloc(len + 1);
	if (!substr)
		return (NULL);
	ft_memcpy(substr, &s[start], len);
	substr[len] = '\0';
	return (substr);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_byte;
	const unsigned char	*src_byte;

	if (!dest || !src)
		return (dest);
	dest_byte = dest;
	src_byte = src;
	while (n > 0)
	{
		*dest_byte++ = *src_byte++;
		n--;
	}
	return (dest);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	n;

	n = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		n = 10 * n + *str - '0';
		str++;
	}
	return (sign * n);
}

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*ft_itoa(int n)
{
	unsigned int	bytes;
	unsigned int	digits;
	int				neg;
	char			*strnum;

	neg = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	digits = ft_how_many_digits(n);
	neg = ft_handle_neg(&n);
	bytes = digits + 1 + neg;
	strnum = (char *) malloc(bytes);
	if (!strnum)
		return (NULL);
	strnum = strnum + bytes - 1;
	*strnum-- = '\0';
	while (digits--)
	{
		*strnum-- = (n % 10) + '0';
		n = n / 10;
	}
	if (neg)
		*strnum-- = '-';
	return (++strnum);
}

unsigned int	ft_how_many_digits(int n)
{
	unsigned int	digits;

	digits = 0;
	if (n < 0)
		n = -n;
	while (1)
	{
		digits++;
		n = n / 10; 
		if (n == 0)
			break ;
	}
	return (digits);
}

int	ft_handle_neg(int *n)
{
	if (*n < 0)
	{
		*n = -*n;
		return (1);
	}
	return (0);
}