/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:14:39 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 18:08:08 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef	struct	s_token	t_token;

typedef	enum e_astkind
{
	AST_CMD,
	AST_PIPE,	
}			t_astkind;

typedef	struct	s_ast
{
	t_astkind		kind;
	struct	s_ast	*left;
	struct	s_ast	*right;
	t_token			*start;
	t_token			*end;
}				t_ast;


t_ast	*parse_line(t_token *toks, int *status);
void	ast_free(t_ast *n);

int	parser_validate(t_token *token);

int	parser_build_pipeline(t_token *toks, t_ast **out_root);

#endif