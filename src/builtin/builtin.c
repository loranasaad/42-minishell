/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:14:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 16:17:17 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return 0 if builtin, else -1
int	builtin_dispatch(char **argv, t_ms *ms)
{
	(void)ms;
	if (!argv || !argv[0])
		return (-1);
	if (strcmp(argv[0], "echo") == 0
		|| strcmp(argv[0], "pwd") == 0
		|| strcmp(argv[0], "env") == 0
		|| strcmp(argv[0], "cd") == 0
		|| strcmp(argv[0], "export") == 0
		|| strcmp(argv[0], "unset") == 0
		|| strcmp(argv[0], "exit") == 0)
		return (0);
	return (-1);
}
