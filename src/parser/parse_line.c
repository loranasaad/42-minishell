/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 17:46:38 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/21 18:04:58 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast	*parse_line(t_token *toks, int *status)
{
	t_ast	*root;
	
	root = NULL;
	if (!toks)
	{
		*status = 0;
		return (NULL);
	}
	if (!parser_validate(toks))
	{
		*status = 2;
		return (NULL);
	}
	if (!parser_build_pipeline(toks, &root))
	{
		*status = 2;
		return (NULL);
	}
	*status = 0;
	return (root);
}
