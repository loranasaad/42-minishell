/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:17:31 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/22 18:24:49 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_C
# define EXEC_C

# include "minishell.h"
# include "parser.h"

int	apply_redirs(const t_redir *rlist);
int	exec_one_cmd(const t_cmdspec *spec, t_ms *ms);

#endif