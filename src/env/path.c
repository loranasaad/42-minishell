/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:41:50 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 12:43:50 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_path_helper(char *name, t_env *env);
static char	*find_pathname(char **paths, char *name);
static char	*find_pathname(char **paths, char *name);

//return the malloc'd command (evtl with path) or NULL when that fails
char	*find_in_path(char *name, t_env *env)
{
	char	*cmd;

	cmd = find_in_path_helper(name, env);
	if (!cmd)
		return (NULL); // cmd not found 127
	return (cmd);
}

static char	*find_in_path_helper(char *name, t_env *env)
{
	char	*path_var;
	char	**paths;

	if (!name)
		return (NULL);
	if (ft_strchr(name, '/'))
	{
		if ((access(name, F_OK) == 0))
			return (ft_strdup(name));
		else
			return (NULL);
	}
	else
	{
		if (!env)
			return (NULL);
		path_var = env_get(env, "PATH");
		if (!path_var)
			return (NULL);
		paths = ft_split(path_var, ':');
		if (!paths)
			return (NULL);
		free_str_arr(&paths);
		return (find_pathname(paths, name));
	}
}

static char	*find_pathname(char **paths, char *name)
{
	int		i;
	char	*pathname;

	i = 0;
	while (paths[i])
	{
		pathname = build_exec_path(paths[i], name);
		if (!pathname)
			return (NULL);
		if (access(pathname, F_OK) == 0)
			return (pathname);
		free(pathname);
		i++;
	}
	return (NULL);
}

static char	*build_exec_path(char const *s1, char const *s2)
{
	size_t	len;
	char	*result;
	char	*dest;

	len = ft_strlen(s1) + ft_strlen(s2) + 2;
	result = (char *) malloc(len);
	if (!result)
		return (NULL);
	dest = result;
	while (*s1)
		*dest++ = *s1++;
	*dest++ = '/';
	while (*s2)
		*dest++ = *s2++;
	*dest = '\0';
	return (result);
}
