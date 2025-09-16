/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:01:42 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/16 16:16:59 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	t_env	*env;

	env = env_init(envp);
	env_set(&env, "poney", "blue", 0);
	env_set(&env, "cat", "red", 1);
	env_set(&env, "OLDPWD", "bla", 0);
	env_set(&env, "PATH", "Ludwigkirchstrasse 2", 1);
	env_set(&env, NULL, "oups", 1);
	env_print_debug(env);
	printf("minishell: stub\n");
	env_free(&env);
	return (0);
}
