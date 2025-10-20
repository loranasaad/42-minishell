/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:14:22 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/19 21:11:41 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// handle expansion of ~ 
// return 0 if builtin, else -1
int	builtin_dispatch(char **argv, t_ms *ms)
{
	(void) ms;

	if (!argv || !argv[0])
		return (-1);
	if (ft_strcmp(argv[0], "echo") == 0
		|| ft_strcmp(argv[0], "exit") == 0
		|| ft_strcmp(argv[0], "cd") == 0
		|| ft_strcmp(argv[0], "pwd") == 0
		|| ft_strcmp(argv[0], "export") == 0
		|| ft_strcmp(argv[0], "unset") == 0
		|| ft_strcmp(argv[0], "env") == 0)
		return (0);
	return (-1);
}

static int	is_n_option(char *arg);
int	builtin_echo(char **argv)
{
	int		newline;
	int		i;
	int		first;

	i = 1;
	newline = 1;
	first = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		if (!first)
			printf(" ");
		printf("%s", argv[i]);
		first = 0;
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	is_n_option(char *arg)
{
	int		i;

	i = 0;
	if (arg[i] != '-')
		return (0);
	i++;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_pwd(void)
{
	char	*cwd;
	char	*error;

	error = "minishell: pwd: error retrieving current directory\n";
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		write(2, error, ft_strlen(error));
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

#include <string.h>
#include <errno.h>
int	builtin_cd(char **argv, t_env *env)
{
	int		status;
	char	*error;
	char	*home_value;
	char	*pwd;
	char	*old_pwd;

	pwd = env_get(env, "PWD");
	old_pwd = env_get(env, "OLD_PWD");
	if (argv[0] && !argv[1])
	{
		home_value = env_get(env, "HOME");
		if (!home_value)
		{
			error = "minishell: cd: HOME not set\n";
			write(2, error, ft_strlen(error));
			return (1);
		}
		status = chdir(home_value);
	}
	else if (argv[0] && argv[1] && ft_strcmp(argv[1], "-") == 0 && !argv[2])
	{
		if (!old_pwd)
		{
			error = "minishell: cd: OLDPWD not set\n";
			write(2, error, ft_strlen(error));
			return (1);
		}
		status = chdir(old_pwd);
	}
	else if (argv[0] && argv[1] && !argv[2])
	{
		status = chdir(argv[1]);
	}
	else
	{
		error = "minishell: cd: too many arguments\n";
		write(2, error, ft_strlen(error));
		return (1);
	}
	if (status != 0)
	{
		error = "minishell: cd: ";
		write(2, error, ft_strlen(error));
		if (argv[1])
		{
			write(2, argv[1], ft_strlen(argv[1]));
			write(2,": ", 2);
		}
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2,"\n", 1);
		return (1);
	}
	char	*new_pwd = getcwd(NULL, 0);
	env_set(&env, "PWD", new_pwd, 1);
	free(new_pwd);
	env_set(&env, "OLD_PWD", pwd, 1);
	return (0);
}

int	builtin_env(t_ms *ms, char **argv)
{
	t_env	*w;

	if (argv[0] && argv[1])
	{
		// write to stderr
		printf("env: %s: No such file or directory\n", argv[1]);
		return (127);
	}
	w = ms->env;
	while (w)
	{
		if (w->key && w->value && w->value[0])
			printf("%s=%s\n", w->key, w->value);
		w = w->next;
	}
	return (0);
}
static int	is_key_valid(char *key);
int	builtin_export(t_ms *ms, char **argv)
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
					env_set(&ms->env, ft_strdup(key_value[0]), ft_strdup(""), 1);
				else
					env_set(&ms->env, ft_strdup(key_value[0]), ft_strdup(key_value[1]), 1);
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

int		builtin_unset(char **argv, t_ms *ms)
{
	int		i;

	i = 1;
	while (argv[i])
	{
		env_unset(&ms->env, argv[i]);
		i++;
	}
	return (0);
}

static int	is_numeric(char *arg);
int		builtin_exit(char **argv, t_ms *ms, int in_parent)
{
	char	*error;

	if (in_parent)
		printf("exit\n"); // only print in parent shell and not when in pipeline
	// argv[0] is not necessary am I paranoid
	if (argv[0] && !argv[1])
	{
		exit (ms->last_status); // 0 or ms->last_status ?
	}
	else if (argv[0] && argv[1] && !argv[2])
	{
		if (is_numeric(argv[1]))
			exit ((unsigned char)ft_atoi(argv[1]));
		else
		{
			// should I write the argument but then function is > 25 lines :( :(
			error = "minishell: exit: numeric argument required\n";
			write(2, error, ft_strlen(error));
			exit (2); // 255 or 2 ?
		}
	}
	else
	{
		error = "minishell: exit: too many arguments\n";
		write(2, error, ft_strlen(error));
		return (1);
	}
}

static int	is_numeric(char *arg)
{
	int		i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (arg[i] >= '0' && arg[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}
