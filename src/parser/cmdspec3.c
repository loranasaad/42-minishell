/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdspec3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:59:57 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 16:02:07 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static char	**strv_push_helper(char **v, char *new_str);

// add the string s to a string array v
char	**strv_push(char **v, const char *s)
{
	int		i;
	char	*new_str;
	char	**new_v;

	if (s)
		new_str = ft_strdup(s);
	else
		new_str = NULL;
	if (!new_str && s)
		return (NULL);
	if (!v)
	{
		new_v = malloc(sizeof(char *) * 2);
		if (!new_v)
		{
			free(new_str);
			return (NULL);
		}
		new_v[0] = new_str;
		new_v[1] = NULL;
		return (new_v);
	}
	return (strv_push_helper(v, new_str));
}

static char	**strv_push_helper(char **v, char *new_str)
{
	int		i;
	char	**new_v;

	new_v = malloc((len_str_arr(v) + 2) * sizeof(char *));
	if (!new_v)
	{
		free(new_str);
		return (NULL);
	}
	i = 0;
	while (v[i])
	{
		new_v[i] = v[i];
		i++;
	}
	new_v[i++] = new_str;
	new_v[i] = NULL;
	free(v);
	return (new_v);
}
