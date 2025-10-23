/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 22:07:21 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/23 18:29:28 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		print_copy(t_env *copy);
static t_env	*copy_env(t_env *env, int *success);
static void		sort_copy(t_env **copy);
static int		swap_it(t_env **before, t_env **current, t_env **after,
					t_env **copy);

int	export_print_env(t_ms *ms)
{
	t_env	*copy;
	int		success;

	success = 1;
	copy = copy_env(ms->env, &success);
	if (!success)
		return (1);
	sort_copy(&copy);
	print_copy(copy);
	env_free(&copy);
	return (0);
}

static void	print_copy(t_env *copy)
{
	t_env	*w;

	w = copy;
	while (w)
	{
		if (w->key && w->has_value)
			printf("declare -x %s=\"%s\"\n", w->key, w->value);
		else if (w->key)
			printf("declare -x %s\n", w->key);
		w = w->next;
	}
}

static t_env	*copy_env(t_env *env, int *success)
{
	t_env	*copy;
	t_env	*w;

	copy = NULL;
	w = env;
	while (w)
	{
		if (add_env_var(&copy, ft_strdup(w->key), ft_strdup(w->value)))
		{
			env_free(&copy);
			perror("minishell: export");
			*success = 0;
			return (NULL);
		}
		w = w->next;
	}
	return (copy);
}

static void	sort_copy(t_env **copy)
{
	int		swap;
	t_env	*current;
	t_env	*before;
	t_env	*after;

	if (!copy || !*copy)
		return ;
	swap = 1;
	while (swap)
	{
		swap = 0;
		current = *copy;
		before = NULL;
		while (current && current->next)
		{
			if (swap_it(&before, &current, &after, copy))
				swap = 1;
		}
	}
}

static int	swap_it(t_env **before, t_env **current, t_env **after,
	t_env **copy)
{
	int	swap;

	swap = 0;
	*after = (*current)->next;
	if (ft_strcmp((*current)->key, (*after)->key) > 0)
	{
		if (!*before)
			*copy = *after;
		else
			(*before)->next = *after;
		(*current)->next = (*after)->next;
		(*after)->next = *current;
		swap = 1;
		*before = *after;
	}
	else
	{
		*before = *current;
		*current = *after;
	}
	return (swap);
}
