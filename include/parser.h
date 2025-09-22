/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:32:46 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/21 18:02:36 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "minishell.h"

typedef enum e_astkind { AST_CMD, AST_PIPE } t_astkind;

typedef struct	s_ast {
	t_astkind		kind;
	struct s_ast	*left;
	struct s_ast	*right;
	t_token			*start;
	t_token			*end;
} t_ast;

typedef struct s_pipeline
{
	t_ast *left;
	t_ast *right;
	t_ast *tmp;
} t_pipeline;

t_ast	*parse_line(t_token *toks, int *status);
void	ast_free(t_ast *n);
int		parser_build_pipeline(t_token *toks, t_ast **out_root);

#endif