/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 14:35:45 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/23 14:56:24 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	hdoc_cleanup(t_redir *r)
{	
	t_redir *current;
	
	current = r;
	while (current)
	{
		if (current->kind == R_HDOC && current->hdoc_fd >= 0)
		{
			close(current->hdoc_fd);
			current->hdoc_fd = -1;
		}
		current = current->next;
	}
}