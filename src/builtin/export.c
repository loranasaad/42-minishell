/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:12:26 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/27 23:57:12 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_key_value(char *arg, int *has_v, char **key, char **value);
static int	get_key_value_helper(char *arg, int *has_v, char **key,
				char **value);
static void	handle_invalid_key(char *key, char *value, int *status);

int	builtin_export(char **argv, t_ms *ms)
{
	char	*key;
	char	*value;
	int		i;
	int		status;
	int		has_value;

	status = 0;
	if (argv[0] && !argv[1])
		return (export_print_env(ms));
	i = 1;
	while (argv[i])
	{
		if (!get_key_value(argv[i], &has_value, &key, &value))
			return (1);
		if (!is_key_valid(key))
			handle_invalid_key(key, value, &status);
		else
			env_set_export(&ms->env, key, value, has_value);
		i++;
	}
	return (status);
}

static void	handle_invalid_key(char *key, char *value, int *status)
{
	char	*s1;
	char	*s2;
	
	s1 = "minishell: export: ";
	s2 = ": not a valid identifier\n";
	write(2, s1, ft_strlen(s1));
	write(2, key, ft_strlen(key));
	write(2, "=", 1);
	write(2, value, ft_strlen(value));
	write(2, s2, ft_strlen(s2));
	free(key);
	free(value);
	*status = 1;
}

static int	get_key_value(char *arg, int *has_value, char **key, char **value)
{
	if (ft_strchr(arg, '='))
	{
		if (!get_key_value_helper(arg, has_value, key, value))
			return (0);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = ft_strdup("");
		*has_value = 0;
	}
	return (1);
}

static int	get_key_value_helper(char *arg, int *has_v, char **key,
	char **value)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	*key = malloc(sizeof(char) * (i + 1));
	if (!*key)
		return (0);
	ft_memcpy(*key, arg, (size_t) i);
	(*key)[i] = '\0';
	*value = ft_strdup(ft_strchr(arg, '=') + 1);
	*has_v = 1;
	return (1);
}
