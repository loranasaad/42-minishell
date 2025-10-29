/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:14:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 13:30:27 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* print is for exit : 1 if single command, 0 in pipeline
   Exit prints "exit" as single cmd and never print in the pipeline */
void	builtin_dispatch(char **argv, t_ms *ms, int *rc, int print)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		*rc = builtin_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		*rc = builtin_pwd(argv);
	else if (ft_strcmp(argv[0], "env") == 0)
		*rc = builtin_env(argv, ms);
	else if (ft_strcmp(argv[0], "cd") == 0)
		*rc = builtin_cd(argv, ms);
	else if (ft_strcmp(argv[0], "export") == 0)
		*rc = builtin_export(argv, ms);
	else if (ft_strcmp(argv[0], "unset") == 0)
		*rc = builtin_unset(argv, ms);
	else if (ft_strcmp(argv[0], "exit") == 0)
		*rc = builtin_exit(argv, ms, print);
}
