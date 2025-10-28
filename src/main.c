/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:01:42 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/26 14:43:18 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_ms	ms;
	
	(void)argc;
	(void)argv;
	(void)envp;
	ms.last_status = 0;
	ms.interactive = isatty(STDIN_FILENO);
	ms.exit_requested = 0;
	ms.env = env_init(envp);
	// termios_disable_echoctl();
	repl(&ms);
	env_free(&(ms.env));
	return ms.last_status;
}
