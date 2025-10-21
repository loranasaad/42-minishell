/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:26 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/20 17:41:49 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// should print in alphabetical order

static int	is_key_valid(char *key);
int	builtin_export(char **argv, t_ms *ms)
{
	t_env	*w;
	char	**key_value;
	int		i;
	int		status;

	status = 0;
	if (argv[0] && !argv[1])
	{
		w = ms->env;
		while (w)
		{
			if (w->key && w->value) // everything or not ?
				printf("declare -x %s=\"%s\"\n", w->key, w->value);
			w = w->next;
		}
		return (status);
	}
	else
	{
		i = 1;
		while (argv[i])
		{
			key_value = ft_split(argv[i], '=');
			if (!is_key_valid(key_value[0]))
			{
				printf("minishell: export: %s: not a valid identifier\n", argv[i]);
				status = 1;
				free_str_arr(&key_value);
				i++;
			}
			else 
			{
				if (!key_value[1])
					env_set(&ms->env, key_value[0], "", 1);
				else
					env_set(&ms->env, key_value[0], key_value[1], 1);
				free_str_arr(&key_value);
				i++;
			}
		}
	}
	return (status);
}

// !!!
#include <ctype.h>

static int	is_key_valid(char *key)
{
	int		i;

	i = 0;
	if (!key[i])
		return (0);
	if (key[i] != '_' && !isalpha(key[i]))
		return (0);
	i++;
	while (key[i])
	{
		if (key[i] != '_' && !isalpha(key[i]) && !isdigit(key[i]))
			return (0);
		i++;
	}
		
	//First char: letter or _
	//Remaining chars: letters, digits, or _
	return (1);
}
