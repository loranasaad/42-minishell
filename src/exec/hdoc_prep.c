/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_prep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:45:44 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:18:03 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>

static	int	hdoc_prepare_one(t_redir *r, t_ms *ms)
{
	int	p[2];
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

/* if (ms->last_status == 0) keep status 130 if interrupt */
static	int	process_redir(t_redir *curr, t_ms *ms, t_redir *r, t_sig_backup *b)
{
	if (curr->kind == R_HDOC)
	{
		if (!hdoc_prepare_one(curr, ms))
		{
			if (ms->last_status == 0)
				ms->last_status = 1;
			hdoc_cleanup(r);
			hdoc_restore_sig(&b->old_sigint, &b->old_sigquit);
			rl_event_hook = b->old_event_hook;
			g_signal = 0;
			return (0);
		}
	}
	return (1);
}

int	hdoc_prepare(t_redir *redirs, t_ms *ms)
{
	t_redir				*current;
	t_sig_backup		backup;

	backup.old_event_hook = rl_event_hook;
	rl_event_hook = hdoc_event;
	hdoc_init_sig(&backup.old_sigint, &backup.old_sigquit);
	g_signal = 0;
	current = redirs;
	while (current)
	{
		if (!process_redir(current, ms, redirs, &backup))
			return (0);
		current = current->next;
	}
	g_signal = 0;
	hdoc_restore_sig(&backup.old_sigint, &backup.old_sigquit);
	rl_event_hook = backup.old_event_hook;
	return (1);
}
