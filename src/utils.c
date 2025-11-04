/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 14:07:01 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:46:11 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>

void	ms_perror(const char *prefix, const char *arg)
{
	const char	*err;
	size_t		len;

	if (prefix)
	{
		len = ft_strlen(prefix);
		write(2, prefix, len);
		write(2, ": ", 2);
	}
	if (arg)
	{
		len = ft_strlen(arg);
		write(2, arg, len);
		write(2, ": ", 2);
	}
	err = strerror(errno);
	len = ft_strlen(err);
	write(2, err, len);
	write(2, "\n", 1);
}
