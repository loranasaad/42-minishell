/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:27:13 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 16:21:17 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parser.h"

typedef	enum	e_tok
{
	TK_WORD,
	TK_PIPE,	// |
	TK_IN,		// <
	TK_OUT,		// >
	TK_APP,		// >>
	TK_HDOC,	// <<
}				t_tok;

typedef struct s_token
{
	t_tok			kind;
	char			*val;
	int				quoted;
	struct	s_token	*next;
}					t_token;


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

//int		termios_disable_echoctl(void);
//void	termios_restore(void);

t_token	*lex_line(const char *line, int *lex_status);

int		match_2(const char *s, int i, char a, char b);
int		is_meta(char c);
int		is_space(char c);
int		word_len(char *str, int *i);

t_token	*tok_new(t_tok kind, char *val, int quoted);
void	tok_push_back(t_token **head, t_token *node);
void	free_tokens(t_token *t);

#endif