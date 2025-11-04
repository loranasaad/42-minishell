/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdoc_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 21:54:41 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 15:18:37 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>

int	hdoc_event(void)
{
	if (g_signal == SIGINT)
		return (1);
	return (0);
}

void	hdoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	rl_done = 1;
	rl_replace_line("", 0);
}

void	hdoc_restore_sig(struct sigaction *oldi, struct sigaction *oldq)
{
	sigaction(SIGINT, oldi, NULL);
	sigaction(SIGQUIT, oldq, NULL);
}

void	hdoc_init_sig(struct sigaction *oldi, struct sigaction *oldq)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, oldi);
	sigaction(SIGQUIT, NULL, oldq);
	sa.sa_handler = hdoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}
