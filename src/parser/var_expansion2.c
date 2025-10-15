/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 16:21:03 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/13 18:19:47 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	is_valid_first_char(char c);
static int	is_valid_char(char c);
static char	*get_normal_segment(char *str, int *i);
static char	*get_expanded_segment(char *str, int *i, t_ms *ms);

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
		else if (is_valid_first_char(str[*i]))
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

static char	*get_normal_segment(char *str, int *i)
{
	char	*result;
	int		start;
	int		end;
	int		j;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	end = *i;
	result = malloc((end - start + 1) * sizeof(char));
	if (!result)
		return (NULL);
	j = 0;
	while (start < end)
		result[j++] = str[start++];
	result[j] = '\0';
	return (result);
}

static char	*get_expanded_segment(char *str, int *i, t_ms *ms)
{
	char	*result;
	char	*key;
	int		start;
	int		end;
	int		j;

	start = *i;
	(*i)++;
	while (is_valid_char(str[*i]))
		(*i)++;
	end = *i;
	key = malloc((end - start + 1) * sizeof(char));
	if (!key)
		return (NULL);
	j = 0;
	while (start < end)
		key[j++] = str[start++];
	key[j] = '\0';
	result = env_get(ms->env, key);
	if (!result)
		result = ft_strdup("");
	else
		result = ft_strdup(result);
	free(key);
	return (result);
}

static int	is_valid_first_char(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_');
}

static int	is_valid_char(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_');
}
