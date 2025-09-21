/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:01:42 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 13:24:36 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_ms	ms;
	
	ms.env = env_init(envp);
	env_set(&(ms.env), "banane", NULL, 1);
	//env_print_debug(ms.env);
	//printf("minishell: stub\n");
	//replt(&ms);
	char **test = env_to_envp(ms.env);
	for (int j = 0; test[j]; j++)
		printf("%s\n", test[j]);
	env_free(&(ms.env));
	return (0);
}
