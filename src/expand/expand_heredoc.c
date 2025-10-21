/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:34:52 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/21 16:23:33 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// how to handle ${USER

// return malloc'd in with var expansion for heredoc
char 	*expand_heredoc_line(char *in, t_ms *ms)
{
	return (expand_dollar_sign(in, ms));
}
