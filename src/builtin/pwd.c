/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:42 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 15:10:57 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv)
{
	char	*cwd;

	if (argv[1])
	{
		write(2, "minishell: pwd: too many arguments\n", 35);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2, "minishell: pwd: error retrieving current directory\n", 51);
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
