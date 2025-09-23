/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:20:57 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/22 21:50:43 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "exec.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	repl(t_ms *ms)
{
	char		*line;
	t_token		*toks;
	t_ast		*ast;
	int			lex_status;
	int			parse_status;
	t_cmdspec	spec;
	extern int rl_catch_signals;
	
	init_prompt_signals();
    rl_catch_signals = 0;
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
		lex_status = 0;
		toks = lex_line(line, &lex_status);
		free(line);
		if (!toks)
		{
			ms->last_status = lex_status;
			continue;
		}
		parse_status = 0;
		ast = parse_line(toks, &parse_status);
		if (!ast)
		{
			ms->last_status = parse_status;
			free_tokens(toks);
			continue;
		}
		if (ast->kind == AST_PIPE)
		{
			//skip on day5
		}
		else if (ast->kind == AST_CMD)
		{
			spec.argv = NULL;
			spec.redirs = NULL;

			if (!build_cmdspec_from_segment(ast->start, ast->end, &spec))
				{
					ms->last_status = 2;
					ast_free(ast);
					free_tokens(toks);
					continue;
				}
			ms->last_status = exec_one_cmd(&spec, ms);
			free_cmdspec(&spec);
		}
		ast_free(ast);
		free_tokens(toks);
		//TODO: execute
	}
}

