/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:20:57 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/18 13:38:48 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void	repl(t_ms *ms)
{
	char	*line;

	(void)ms;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break;
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

