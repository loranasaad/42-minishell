/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:20:21 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/05 16:51:51 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// return malloced str of input with var expansion; status is 0 if malloc failed
char	*handle_var_expansion(char *str, int quoted, t_ms *ms, int *status)
{
	char	*result;

	result = NULL;
	if (!str)
		return (str);
	if (!str[0])
		return (ft_strdup(""));
	if (quoted == 0 || quoted == 2)
		result = expand_dollar_sign(str, ms);
	else if (quoted == 1)
		result = ft_strdup(str);
	if (!result)
		*status = 0;
	else
		*status = 1;
	return (result);
}

char	*expand_dollar_sign(char *str, t_ms *ms)
{
	int		i;
	int		end;
	char	*result;
	char	*segment;
	char	*tmp;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (1)
	{
		segment = get_next_segment(str, &i, ms, &end);
		if (!segment && end)
			break ;
		if (!segment && !end)
		{
			free(result);
			result = NULL;
			break ;
		}
		tmp = result;
		result = ft_strjoin(result, segment);
		free(tmp);
		free(segment);
		if (!result)
			break ;
	}
	return (result);
}
