/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:09:58 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 15:22:30 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo '' => unitialized value somewhere
// echo ~ => expand ~ in variable expansion

static int	is_n_option(char *arg);

int	builtin_echo(char **argv)
{
	int		newline;
	int		i;
	int		first;

	i = 1;
	newline = 1;
	first = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		if (!first)
			printf(" ");
		printf("%s", argv[i]);
		first = 0;
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	is_n_option(char *arg)
{
	int		i;

	i = 0;
	if (arg[i] != '-')
		return (0);
	i++;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
