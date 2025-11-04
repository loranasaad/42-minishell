/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_fill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:15:54 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:16:54 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>

static	int	hdoc_write_line(int wfd, char *line, int expand, t_ms *ms)
{
	char	*out;
	size_t	len;

	out = line;
	if (expand)
	{
		out = expand_dollar_sign(line, ms);
		free(line);
		if (!out)
			return (0);
	}
	len = ft_strlen(out);
	if (write(wfd, out, len) < 0 || write(wfd, "\n", 1) < 0)
	{
		free(out);
		return (0);
	}
	free(out);
	return (1);
}

static	int	hdoc_handle_eof(char *limiter, t_ms *ms)
{
	if (g_signal == SIGINT)
	{
		ms->last_status = 130;
		return (0);
	}
	write(2, "minishell: warning: ", 20);
	write(2, "here-document delimited by end-of-file (wanted '", 48);
	write(2, limiter, ft_strlen(limiter));
	write(2, "')\n", 3);
	return (1);
}

int	hdoc_fill(int wfd, char *limiter, int expand, t_ms *ms)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (hdoc_handle_eof(limiter, ms));
		if (g_signal == SIGINT)
		{
			free(line);
			ms->last_status = 130;
			return (0);
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			return (1);
		}
		if (!hdoc_write_line(wfd, line, expand, ms))
			return (0);
	}
}
