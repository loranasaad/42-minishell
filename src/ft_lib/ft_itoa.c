/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:35:19 by latabagl          #+#    #+#             */
/*   Updated: 2025/11/04 15:53:30 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	ft_how_many_digits(int n)
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
