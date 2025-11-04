/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:13:25 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/29 14:18:07 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cleanup(t_cu *cleanup, t_ast *ast, t_token *toks)
{
	cleanup->ast = ast;
	cleanup->toks = toks;
	cleanup->spec = NULL;
	cleanup->stages = NULL;
	cleanup->specs = NULL;
	cleanup->pipes = NULL;
	cleanup->pids = NULL;
	cleanup->pipe_len = 0;
}
