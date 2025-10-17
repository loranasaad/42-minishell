/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:17:31 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/17 16:41:13 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_C
# define EXEC_C

# include "minishell.h"
# include "parser.h"

int	apply_redirs(const t_redir *rlist);
int	exec_one_cmd(const t_cmdspec *spec, t_ms *ms);

int	exec_run_node(t_ast *node, t_ms *ms);
int	exec_pipeline(t_ast *root, t_ms *ms);

int	status_to_rc(int st);
void exec_error(const char *cmd);
int	is_stateful(const char *name);
int	is_builtin(const char *name);

#endif