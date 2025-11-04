/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:21:03 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 18:16:40 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_char(char c, int first)
{
	if (first)
	{
		return ((c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| c == '_');
	}
	else
	{
		return ((c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9')
			|| c == '_');
	}
}

static char	*get_normal_segment(char *str, int *i)
{
	int		start;
	int		end;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	end = *i;
	return (ft_substr(str, start, end - start));
}

static char	*get_variable_name(char *str, int *i)
{
	int		start;
	int		end;

	if (str[*i] == '{')
	{
		(*i)++;
		start = *i;
		while (str[*i] && str[*i] != '}')
			(*i)++;
		if (!str[*i])
		{
			write(2, "minishell: syntax error: \
					unexpected end of variable expansion\n", 62);
			return (NULL);
		}
		end = (*i)++;
		return (ft_substr(str, start, end - start));
	}
	else
	{
		start = (*i)++;
		while (is_valid_char(str[*i], 0))
			(*i)++;
		end = *i;
		return (ft_substr(str, start, end - start));
	}
}

static char	*get_expanded_segment(char *str, int *i, t_ms *ms)
{
	char	*result;
	char	*key;

	key = get_variable_name(str, i);
	if (!key)
		return (NULL);
	result = env_get(ms->env, key);
	if (!result)
		result = ft_strdup("");
	else
		result = ft_strdup(result);
	free(key);
	return (result);
}

char	*get_next_segment(char *str, int *i, t_ms *ms, int *end)
{
	*end = 0;
	if (!str[*i])
		return (*end = 1, NULL);
	else if (str[*i] == '$')
	{
		(*i)++;
		if (str[*i] == '?')
		{
			(*i)++;
			return (ft_itoa(ms->last_status));
		}
		else if (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\0')
			return (ft_strdup("$"));
		else if (is_valid_char(str[*i], 1) || str[*i] == '{')
			return (get_expanded_segment(str, i, ms));
		else
		{
			(*i)++;
			return (ft_strdup(""));
		}
	}
	else
		return (get_normal_segment(str, i));
}
