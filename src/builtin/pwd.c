/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:42 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/27 15:22:07 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv)
{
	char	*cwd;

	(void) argv;
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
