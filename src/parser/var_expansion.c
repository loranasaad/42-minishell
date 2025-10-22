/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 12:20:21 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/15 16:20:41 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static char	*join_res_seg(char *result, char *segment);

// {$VAR}  should we deal with that ?

// return malloced str of input with var expansion; status is 0 if malloc failed or { not closed
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

	i = 0;
	result = ft_strdup("");
	while (1)
	{
		if (!result)
			return (NULL);
		segment = get_next_segment(str, &i, ms, &end);
		if (!segment && end)
			break ;
		if (!segment && !end)
		{
			free(result);
			result = NULL;
			break ;
		}
		result = join_res_seg(result, segment);
	}
	return (result);
}

static char	*join_res_seg(char *result, char *segment)
{
	char	*tmp;

	tmp = result;
	result = ft_strjoin(result, segment);
	free(tmp);
	free(segment);
	return (result);
}
