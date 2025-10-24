/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:24:21 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/23 13:39:05 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include <fcntl.h>

static	int do_hdoc(t_redir *r)
{
	if (r->hdoc_fd < 0)
	{
		ms_perror("minishell", "heredoc");
		return(0);
	}
	if (dup2(r->hdoc_fd, STDIN_FILENO) < 0)
	{
		ms_perror("minishell", "dup2");
		close(r->hdoc_fd);
		r->hdoc_fd = -1;
		return (0);
	}
	if (close(r->hdoc_fd) < 0)
	{
		ms_perror("minishell", "close");
	}
	r->hdoc_fd = -1;
	return (1);
}

static int	do_redir(const char *path, int flags, int target_fd)
{
	int	fd;
	
	if (flags & O_CREAT)				// R_OUT R_APP
		fd = open(path, flags, 0644);
	else
		fd = open(path, flags);			//R_IN
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
	}
	return (1);
}

static	int	apply_one_redir(t_redir *r)
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
		if (!do_hdoc(r))
			return (0);
	}
	return (1);
}

int	apply_redirs(t_redir *r)
{
	t_redir *current;

	current = r;
	while (current)
	{
		if (!apply_one_redir(current))
			return (0);
		current = current->next;
	}
	return (1);
}

