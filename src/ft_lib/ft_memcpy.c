/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:41:05 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/22 15:50:34 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
