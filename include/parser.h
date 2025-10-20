/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:21:44 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/20 15:20:32 by loasaad          ###   ########.fr       */
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

typedef struct s_pipeline
{
	t_ast *left;
	t_ast *right;
	t_ast *tmp;
} t_pipeline;


typedef enum	e_rkind
{
	R_IN,
	R_OUT,
	R_APP,
	R_HDOC,
}				t_rkind;

typedef struct s_redir
{
	t_rkind			kind;
	char 			*target;
	int				hdoc_exp;
	int				hdoc_fd;
	struct	s_redir	*next;
}					t_redir;

typedef	struct	s_cmdspec
{
	char	**argv;
	t_redir	*redirs;
}			t_cmdspec;


int		build_cmdspec_from_segment(t_token *start, t_token *end, t_cmdspec *out);
void	free_cmdspec(t_cmdspec *spec);

t_ast	*parse_line(t_token *toks, int *status);
void	ast_free(t_ast *n);

int	parser_validate(t_token *token);

int	parser_build_pipeline(t_token *toks, t_ast **out_root);


#endif