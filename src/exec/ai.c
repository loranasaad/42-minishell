/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:45:44 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/22 20:45:12 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>
#include <unistd.h>
#include <string.h>

/* ===========================
   = Signal helpers =
   =========================== */

/*
** hdoc_sig_enter:
** Temporarily set heredoc signal behavior.
**   - SIGINT → default (so Ctrl-C cancels heredoc)
**   - SIGQUIT → ignored
** Also save previous handlers in *old_i and *old_q,
** so we can restore them later.
*/
static void	hdoc_sig_enter(struct sigaction *old_i, struct sigaction *old_q)
{
	struct sigaction sa;

	sigaction(SIGINT, NULL, old_i);
	sigaction(SIGQUIT, NULL, old_q);
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/*
** hdoc_sig_leave:
** Restore original signal handlers for SIGINT and SIGQUIT.
** Called once heredoc reading is done (success or fail).
*/
static void	hdoc_sig_leave(struct sigaction *old_i, struct sigaction *old_q)
{
	sigaction(SIGINT, old_i, NULL);
	sigaction(SIGQUIT, old_q, NULL);
}

/* ===========================
   = Utility cleanup =
   =========================== */

/*
** hdoc_close_list:
** Close all open heredoc file descriptors in a redir list.
** Called on error or after the pipeline is done.
*/
static void	hdoc_close_list(t_redir *r)
{
	while (r)
	{
		if (r->kind == R_HDOC && r->hdoc_fd >= 0)
		{
			close(r->hdoc_fd);
			r->hdoc_fd = -1;
		}
		r = r->next;
	}
}

/* ===========================
   = Line writing helpers =
   =========================== */

/*
** hdoc_write_line:
** Write one line + newline to the heredoc's write-end.
** If expand == 1 → expand variables before writing.
** Otherwise write as-is.
*/
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
		if (expand)
			free(out);
		return (0);
	}
	if (expand)
		free(out);
	return (1);
}

/*
** hdoc_fill:
** Read lines interactively until limiter is reached.
** - Each line is written to heredoc pipe.
** - Stops when line == limiter or EOF/Ctrl-C.
** Returns 1 on success, 0 on error or interrupt.
*/
static int	hdoc_fill(int wfd, const char *lim, int expand, t_ms *ms)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal == SIGINT)
				ms->last_status = 130;
			return (0);
		}
		if (ft_strcmp(line, lim) == 0)
		{
			free(line);
			return (1);
		}
		if (!hdoc_write_line(wfd, line, expand, ms))
			return (0);
	}
}

/* ===========================
   = Core heredoc logic =
   =========================== */

/*
** hdoc_prepare_one:
** Create a pipe for this heredoc, fill it by reading from stdin.
** - Write all user input to the write end.
** - Close write end when done.
** - Store read end in redir->hdoc_fd.
** Returns 1 if OK, 0 if interrupted or failed.
*/
static int	hdoc_prepare_one(t_redir *r, t_ms *ms)
{
	int p[2];
	int ok;

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

/*
** hdoc_prepare:
** Public entry point.
** For every heredoc redirection in the list:
**   - Temporarily change signals (Ctrl-C cancels heredoc)
**   - Read heredoc content into a pipe
**   - Store pipe read end in redir->hdoc_fd
** On error or Ctrl-C:
**   - close all prepared fds
**   - restore signals
**   - set ms->last_status = 1 (or keep 130 if interrupt)
*/
int	hdoc_prepare(t_redir *redirs, t_ms *ms)
{
	struct sigaction	old_i;
	struct sigaction	old_q;
	t_redir				*cur;
	int					ok;

	hdoc_sig_enter(&old_i, &old_q);
	cur = redirs;
	while (cur)
	{
		if (cur->kind == R_HDOC)
		{
			cur->hdoc_fd = -1;
			ok = hdoc_prepare_one(cur, ms);
			if (!ok)
			{
				if (ms->last_status == 0)
					ms->last_status = 1;
				hdoc_close_list(redirs);
				hdoc_sig_leave(&old_i, &old_q);
				return (0);
			}
		}
		cur = cur->next;
	}
	hdoc_sig_leave(&old_i, &old_q);
	return (1);
}