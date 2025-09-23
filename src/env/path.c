/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:41:50 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 18:19:17 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_path_helper(const char *name, t_env *env);
static char	*find_pathname(char **paths, const char *name);
static char	*build_exec_path(char const *dir, char const *name);


//return the malloc'd command (evtl with path) or NULL when that fails
char	*find_in_path(const char *name, t_env *env)
{
	char	*cmd;

	cmd = find_in_path_helper(name, env);
	if (!cmd)
		return (NULL); // cmd not found 127 LORAN: this will be handled by the caller
	return (cmd);
}

static char	*find_in_path_helper(const char *name, t_env *env)
{
	const	char	*path_var;
	char			**paths;
	char			*resolved;

	if (!name || !*name)
		return (NULL);
	if (ft_strchr(name, '/'))
		return (ft_strdup(name));
	else
	{
		if (!env)
			return (NULL);
		path_var = env_get(env, "PATH");
		if (!path_var || !*path_var)
			return (NULL);
		paths = ft_split(path_var, ':');
		if (!paths)
			return (NULL);
		resolved = find_pathname(paths, name);
		free_str_arr(&paths);
		return (resolved);
	}
}

static char	*find_pathname(char **paths, const char *name)
{
	int				i;
	char			*pathname;
	const	char	*dir;

	i = 0;
	while (paths[i])
	{
		if (paths[i][0] == '\0') 			// empty path = current dir
			dir = ".";
		else
			dir = paths[i];
		pathname = build_exec_path(dir, name);
		if (!pathname)
			return (NULL);
		if (access(pathname, X_OK) == 0) 	//x_OK instead of F_OK
			return (pathname);				// caller frees
		free(pathname);
		i++;
	}
	return (NULL);
}

static char	*build_exec_path(char const *dir, char const *name)
{
	size_t	len_dir;
	size_t	len_name;
	char	*result;
	char	*p;

	len_dir = ft_strlen(dir);
	len_name = ft_strlen(name);
	result = (char *) malloc(len_dir + len_name + 2);
	if (!result)
		return (NULL);
	p = result;
	ft_memcpy(p, dir, len_dir);
	p += len_dir;
	if (len_dir == 0 || dir[len_dir -1] != '/')
		*p++ = '/';
	ft_memcpy(p, name, len_name);
	p += len_name;
	*p = '\0';
	return (result);
}
