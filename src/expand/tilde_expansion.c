/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:12:20 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 18:16:35 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_tilde(char *str, int quoted)
{
	if (quoted != 0)
		return (0);
	if (str[0] != '~')
		return (0);
	if (str[1] == '\0' || str[1] == '/')
		return (1);
	return (0);
}

static char	*expanded_tilde(char *str, t_ms *ms)
{
	char	*home;

	home = env_get(ms->env, "HOME");
	if (!home)
		return (ft_strdup(str));
	return (ft_strjoin(home, str + 1));
}

/* return malloced str with tilde expansion or not
tilde expands to HOME when :
quoted == 0
its the first character
and afterwards there is the null character or / */
char	*expand_tilde(char *str, int quoted, t_ms *ms)
{
	if (is_tilde(str, quoted))
		return (expanded_tilde(str, ms));
	return (ft_strdup(str));
}
