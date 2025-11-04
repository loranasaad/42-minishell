/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:20:57 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/29 14:16:03 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static	int	sigint_repl_handler(char *line, t_ms *ms)
{
	if (g_signal == SIGINT)
	{
		ms->last_status = 130;
		g_signal = 0;
		if (line)
			free(line);
		return (1);
	}
	return (0);
}

static t_token	*lex_input(char *line, int *lex_status, t_ms *ms)
{
	t_token		*toks;

	*lex_status = 0;
	toks = lex_line(line, &lex_status);
	free(line);
	if (!toks)
		ms->last_status = lex_status;
	return (toks);
}

static t_ast	*parse_tokens(t_token *toks, int *parse_status, t_ms *ms)
{
	t_ast	*ast;

	*parse_status = 0;
	ast = parse_line(toks, &parse_status);
	if (!ast)
	{
		ms->last_status = parse_status;
		free_tokens(toks);
	}
	return (ast);
}

static	int	process_line(char *line, t_ms *ms)
{
	t_token		*toks;
	t_ast		*ast;
	int			lex_status;
	int			parse_status;
	t_cu		cleanup;

	if (sigint_repl_handler(line, ms))
		return (0);
	if (line[0] == '\0')
	{
		free(line);
		return (0);
	}
	add_history(line);
	toks = lex_input(line, &lex_status, ms);
	if (!toks)
		return (0);
	ast = parse_tokens(toks, &parse_status, ms);
	if (!ast)
		return (0);
	init_cleanup(&cleanup, ast, toks);
	ms->last_status = exec_run_node(ast, ms, &cleanup);
	ast_free(ast);
	free_tokens(toks);
	return (ms->exit_requested);
}

void	repl(t_ms *ms)
{
	char		*line;
	extern int	rl_catch_signals;

	init_prompt_signals();
	rl_catch_signals = 0;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			clear_history();
			break ;
		}
		if (process_line(line, ms))
		{
			clear_history();
			break ;
		}
	}
}
