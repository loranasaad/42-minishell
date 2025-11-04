/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:49:35 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 14:59:34 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	prep_cleanup(t_pipe_data *data, t_cu *cu, int i)
{
	cu->spec = &data->specs[i];
	cu->stages = data->stages;
	cu->specs = data->specs;
	cu->pipes = data->pipes;
	cu->pids = data->pids;
	cu->pipe_len = data->len;
}
