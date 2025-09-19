/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:01:42 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/18 15:50:43 by latabagl         ###   ########.fr       */
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
	char **en = env_to_envp(env);
	int	i = 0;
	while (en[i])
	{
		printf("%s\n", en[i]);
		i++;
	}
	printf("minishell: stub\n");
	env_free(&env);
	return (0);
}
