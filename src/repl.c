/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:20:57 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/20 14:20:24 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	repl(t_ms *ms)
{
	char	*line;

	init_prompt_signals();
	extern int rl_catch_signals;
    rl_catch_signals = 0;
	(void)ms;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			// termios_restore();
			break;
		}
		if (g_signal == SIGINT)
		{
			ms->last_status = 130;
			g_signal = 0;
			if (line)
				free(line);
			continue;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue;
		}
		add_history(line);
		//TODO: parse
		//TODO: execute
		free (line);
	}
}

