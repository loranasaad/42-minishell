/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:14:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/17 17:03:26 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_dispatch(char **argv, t_ms *ms, int *rc)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		*rc = builtin_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		*rc = builtin_pwd();
	else if (ft_strcmp(argv[0], "env") == 0)
		*rc = builtin_env(ms->env);
	else if (ft_strcmp(argv[0], "cd") == 0)
		*rc = builtin_cd(argv, ms);
	else if (ft_strcmp(argv[0], "export") == 0)
		*rc = builtin_export(argv, ms);
	else if (ft_strcmp(argv[0], "unset") == 0)
		*rc = builtin_unset(argv, ms);
	else if (ft_strcmp(argv[0], "exit") == 0)
		*rc = builtin_exit(argv, ms);
}

