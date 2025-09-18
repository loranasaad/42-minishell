/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:27:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/18 13:31:48 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct	s_env	*next;
}					t_env;

typedef struct s_ms
{
	t_env	*env;
	int		last_status;
	int		interactive;
}			t_ms;

extern	int	g_signal;

void	repl(t_ms *ms);

void	init_prompt_signals(void);

#endif