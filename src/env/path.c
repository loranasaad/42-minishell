/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:41:50 by latabagl          #+#    #+#             */
/*   Updated: 2025/09/23 19:08:52 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_path_helper(char const *name, t_env *env);
static char	*find_pathname(char **paths, char const *name);
static char	*build_exec_path(char const *dir, char const *name);


/*return the malloc'd command (evtl with path) or NULL when that fails
F_OK => check if something exist
X_OK => check if something is executable
IMO checks about does the command exist or is executable should be handled 
by the caller of find_in_path
to be able to print the right error message
Or that could be added to find_in_path
*/
char	*find_in_path(char const *name, t_env *env)
{
	char	*cmd;

	cmd = find_in_path_helper(name, env);
	if (!cmd)
		return (NULL);
	return (cmd);
}

static char	*find_in_path_helper(char const *name, t_env *env)
{
	char	*path_var;
	char	**paths;
	char	*resolved;

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

static char	*find_pathname(char **paths, char const *name)
{
	int		i;
	char	*pathname;
	char const	*dir;

	i = 0;
	while (paths[i])
	{
		if (paths[i][0] == '\0')
			dir = ".";
		else
			dir = paths[i];
		pathname = build_exec_path(dir, name);
		if (!pathname)
			return (NULL);
		if (access(pathname, F_OK) == 0)
			return (pathname);
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
	char	*dest;

	len_dir = ft_strlen(dir);
	len_name = ft_strlen(name);
	result = (char *) malloc(len_dir + len_name + 2);
	if (!result)
		return (NULL);
	dest = result;
	ft_memcpy(dest, dir, len_dir);
	dest += len_dir;
	if (len_dir == 0 || dir[len_dir -1] != '/')
		*dest++ = '/';
	ft_memcpy(dest, name, len_name);
	dest += len_name;
	*dest = '\0';
	return (result);
}
