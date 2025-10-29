/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:11:19 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 16:57:48 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	general_error_msg(char *arg)
{
	write(2, "minishell: cd: ", 15);
	if (arg)
	{
		write(2, arg, ft_strlen(arg));
		write(2, ": ", 2);
	}
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	return (1);
}

static int	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

static int	builtin_cd_helper(char **argv, t_ms *ms, int *status)
{
	char	*old_pwd;
	char	*home_value;

	old_pwd = env_get(ms->env, "OLDPWD");
	if (!argv[1])
	{
		home_value = env_get(ms->env, "HOME");
		if (!home_value)
			return (error_msg("minishell: cd: HOME not set\n"));
		*status = chdir(home_value);
	}
	else if (argv[1] && ft_strcmp(argv[1], "-") == 0 && !argv[2])
	{
		if (!old_pwd)
			return (error_msg("minishell: cd: OLDPWD not set\n"));
		printf("%s\n", old_pwd);
		*status = chdir(old_pwd);
	}
	else if (argv[1] && !argv[2])
		*status = chdir(argv[1]);
	else
		return (error_msg("minishell: cd: too many arguments\n"));
	return (0);
}

int	builtin_cd(char **argv, t_ms *ms)
{
	int		status;
	int		error;
	char	*pwd;
	char	*new_pwd;

	status = 0;
	pwd = env_get(ms->env, "PWD");
	error = builtin_cd_helper(argv, ms, &status);
	if (error)
		return (1);
	if (status != 0)
		return (general_error_msg(argv[1]));
	env_set(&ms->env, "OLDPWD", pwd, 1);
	new_pwd = getcwd(NULL, 0);
	env_set(&ms->env, "PWD", new_pwd, 1);
	free(new_pwd);
	return (0);
}
