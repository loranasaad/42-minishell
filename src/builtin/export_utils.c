/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 22:03:27 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 17:05:44 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_isalpha(char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	add_env_var_export(t_env **env, char *key, char *value, int has_v)
{
	t_env	*var;
	t_env	*tmp;

	var = malloc(sizeof(t_env));
	if (!var)
		return (free(key), free(value), 1);
	var->key = key;
	var->value = value;
	var->has_value = has_v;
	var->next = NULL;
	if (!*env)
		*env = var;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = var;
	}
	return (0);
}

int	env_set_export(t_env **env, char *key, char *value, int has_value)
{
	t_env	*tmp;

	if (!key || !env || !value)
		return (free(key), free(value), 1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, key) == 0)
		{
			if (has_value)
			{
				free(tmp->value);
				tmp->value = value;
				tmp->has_value = 1;
			}
			free(key);
			if (!has_value)
				free(value);
			return (0);
		}
		tmp = tmp->next;
	}
	return (add_env_var_export(env, key, value, has_value));
}

// First char: letter or _, Remaining chars: letters, digits, or _
int	is_key_valid(char *key)
{
	int	i;

	i = 0;
	if (!key[i])
		return (0);
	if (key[i] != '_' && !ft_isalpha(key[i]))
		return (0);
	i++;
	while (key[i])
	{
		if (key[i] != '_' && !ft_isalpha(key[i]) && !ft_isdigit(key[i]))
			return (0);
		i++;
	}
	return (1);
}
