/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:24:21 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/23 00:35:40 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <fcntl.h>

static int	do_redir(const char *path, int flags, int target_fd)
{
	int	fd;
	
	if (flags & O_CREAT)
		fd = open(path, flags, 0644);
	else
		fd = open(path, flags);
	if (fd < 0)
	{
		ms_perror("minishell", path);
		return (0);
	}
	if (dup2(fd, target_fd) < 0)
	{
		ms_perror("minishell", "dup2");
		close(fd);
		return (0);
	}
	if (close(fd) < 0)
	{
		ms_perror("minishell", "close");
		return (0);
	}
	return (1);
}

int	apply_redirs(const t_redir *rlist)
{
	const	t_redir *r;
	
	r = rlist;
	while (r)
	{
		if (r->kind == R_IN)
		{
			if (!do_redir(r->target, O_RDONLY, STDIN_FILENO))
				return(0);
		}
		else if (r->kind == R_OUT)
		{
			if (!do_redir(r->target, O_WRONLY | O_CREAT | O_TRUNC, STDOUT_FILENO))
				return(0);
		}
		else if (r->kind == R_APP)
		{
			if (!do_redir(r->target, O_WRONLY | O_CREAT | O_APPEND, STDOUT_FILENO))
				return(0);
		}
		else if (r->kind == R_HDOC)
		{
			write(2, "minishell: heredoc: not implemented\n", 36);
			return (0);
		}
		r = r->next;
	}
	return (1);
}

