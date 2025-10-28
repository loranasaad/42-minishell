/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:45:44 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/28 10:47:11 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>

extern int	rl_done;

/* Event hook - called by readline periodically to check for events */
static int	hdoc_event(void)
{
	if (g_signal == SIGINT)
		return (1);  // Tell readline to abort
	return (0);
}

static void hdoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
}

static void	restore_sig(struct sigaction *oldi, struct sigaction *oldq)
{
	sigaction(SIGINT, oldi, NULL);
	sigaction(SIGQUIT, oldq, NULL);
}

static void	init_sig(struct sigaction *oldi, struct sigaction *oldq)
{
	struct sigaction sa;
	
	//save old signals
	sigaction(SIGINT, NULL, oldi);
	sigaction(SIGQUIT, NULL, oldq);

	sa.sa_handler = hdoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	//ignore sigquit
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

static int	hdoc_write_line(int wfd, char *line, int expand, t_ms *ms)
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

static int	hdoc_fill(int wfd, char *limiter, int expand, t_ms *ms)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		
		// Check for NULL (EOF or interrupted)
		if (!line)
		{
			// Check if interrupted by Ctrl+C
			if (g_signal == SIGINT)
			{
				ms->last_status = 130;
				return (0);
			}
			// Otherwise it's EOF (Ctrl+D)
			write(2, "minishell: warning: here-document delimited by end-of-file (wanted `", 69);
			write(2, limiter, ft_strlen(limiter));
			write(2, "')\n", 3);
			return (1);
		}
		
		// Check if interrupted but readline returned non-NULL
		if (g_signal == SIGINT)
		{
			free(line);
			ms->last_status = 130;
			return (0);
		}
		
		// Check if delimiter reached
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			return (1);
		}
		
		// Write line to pipe
		if (!hdoc_write_line(wfd, line, expand, ms))
			return (0);
	}
}

static int	hdoc_prepare_one(t_redir *r, t_ms *ms)
{
	int p[2];
	int	ok;
	
	if (pipe(p) < 0)
	{
		ms_perror("minishell", "pipe");
		return (0);
	}
	ok = hdoc_fill(p[1], r->target, r->hdoc_exp, ms);
	close(p[1]);
	if (!ok)
	{
		close(p[0]);
		return (0);
	}
	r->hdoc_fd = p[0];
	return (1);
}

int	hdoc_prepare(t_redir *redirs, t_ms *ms)
{
	struct sigaction	oldi;
	struct sigaction	oldq;
	t_redir				*current;
	int					(*old_event_hook)(void);
	
	// Save and install event hook
	old_event_hook = rl_event_hook;
	rl_event_hook = hdoc_event;
	
	init_sig(&oldi, &oldq);
	g_signal = 0;
	current = redirs;
	while (current)
	{
		if (current->kind == R_HDOC)
		{
			if (!hdoc_prepare_one(current, ms))
			{
				if (ms->last_status == 0)
					ms->last_status = 1;
				hdoc_cleanup(redirs);
				restore_sig(&oldi, &oldq);
				rl_event_hook = old_event_hook;
				g_signal = 0;
				return (0);
			}
		}
		current = current->next;
	}
	g_signal = 0;
	restore_sig(&oldi, &oldq);
	rl_event_hook = old_event_hook;
	return (1);
}