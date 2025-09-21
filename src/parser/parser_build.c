/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 16:37:02 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/21 18:39:03 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static t_ast	*ast_new_cmd(t_token *start, t_token *end)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->kind = AST_CMD;
	node->left = NULL;
	node->right = NULL;
	node->start = start;
	node->end = end;
	return (node);
}
static t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->kind = AST_PIPE;
	node->left = left;
	node->right = right;
	node->start = NULL;
	node->end = NULL;
	return (node);
}

// 0 : no more segment, 1 : produced a segment
static int	next_segment(t_token **it, t_token **out_start, t_token **out_end)
{
	t_token	*w;

	w = *it;
	if (!w)
		return (0);
	*out_start = w;
	while (w && w->kind != TK_PIPE)
		w = w->next;
	*out_end = w;
	if (w)
		*it = w->next;
	else
		*it = NULL;
	return (1);
}

static void	free_little_tree(t_ast	*node)
{
	if (!node)
		return ;
	free_little_tree(node->left);
	free_little_tree(node->right);
	free(node);
	return ;
}

// return 0 when error (malloc fails), 1 when success
int parser_build_pipeline(t_token *toks, t_ast **out_root)
{
	t_token		*start;
	t_token		*end;
	t_token		*it;
	t_pipeline	p;

	it = toks;

	*out_root = NULL;
	if (!next_segment(&it, &start, &end))
		return (1);
	p.left = ast_new_cmd(start, end);
	if (!p.left)
		return (0);
	while (next_segment(&it, &start, &end))
	{
		p.right = ast_new_cmd(start, end);
		if (!p.right)
		{
			free_little_tree(p.left);
			return (0);
		}
		p.tmp = ast_new_pipe(p.left, p.right);
		if (!p.tmp)
		{
			free_little_tree(p.right);
			free_little_tree(p.left);
			return (0);
		}
		p.left = p.tmp;
	}
	*out_root = p.left;
	return (1);
}

