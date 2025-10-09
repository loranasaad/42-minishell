/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:44:36 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/06 19:46:15 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// return 1 if field splitting is necessary
int	several_fields(int quoted, char	*val)
{
	if (quoted == 0
	&& (ft_strchr(val, ' ')
		|| ft_strchr(val, '\t')
		|| ft_strchr(val, '\n')))
		return (1);
	return (0);
}

//field split the thing, return 1 if malloc was ok
int	field_split(char *val, t_cmdspec *out)
{
	char	*arg;
	int		start;
	int		end;
	int 	i;
	int		j;

	start = 0;
	i = 0;
	while (val[i])
	{
		if (val[i] == '\t' || val[i] == ' ' || val[i] == '\n')
			i++;
		else
		{
			start = i;
			while (val[i] && val[i] != '\t' && val[i] != ' ' && val[i] != '\n')
				i++;
			end = i;
			arg = malloc(sizeof(char) * (end - start + 1));
			if (!arg)
				return (0);
			j = 0;
			while (start < end)
				arg[j++] = val[start++];
			arg[j] = '\0';
			out->argv = strv_push(out->argv, arg);
			free(arg);
			if (!out->argv)
				return (0);
		}
	}
	return (1);
}
