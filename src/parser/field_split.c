/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 11:44:36 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/28 13:30:37 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	add_arg(int start, int end, char *val, t_cmdspec *out);

// return 1 if field splitting is necessary
int	several_fields(int quoted, char	*val)
{
	if (quoted != 0)
		return (0) ;
	if ((ft_strchr(val, ' ')
			|| ft_strchr(val, '\t') || ft_strchr(val, '\n')))
		return (1);
	return (0);
}

//field split the thing, return 0 if malloc failed
int	field_split(char *val, t_cmdspec *out)
{
	int		start;
	int		end;
	int		i;

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
			if (!add_arg(start, end, val, out))
				return (0);
		}
	}
	return (1);
}

static int	add_arg(int start, int end, char *val, t_cmdspec *out)
{
	char	*arg;

	arg = ft_substr(val, start, end - start);
	if (!arg)
		return (0);
	out->argv = strv_push(out->argv, arg);
	free(arg);
	if (!out->argv)
		return (0);
	return (1);
}
