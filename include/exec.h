/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:29:44 by latabagl          #+#    #+#             */
/*   Updated: 2025/10/29 18:07:15 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

int		exec_run_node(t_ast *node, t_ms *ms, t_cu *cleanup);
int		exec_one_cmd(t_cmdspec *spec, t_ms *ms, t_cu *cleanup);
int		apply_redirs(t_redir *rlist);
int		exec_pipeline(t_ast *root, t_ms *ms, t_cu *cleanup);
void	child_cleanup_all(t_ms *ms, t_cu *cleanup);
int		status_to_rc(int st);
void	exec_error(const char *cmd);
int		is_stateful(const char *name);
int		is_builtin(const char *name);
int		hdoc_prepare(t_redir *redirs, t_ms *ms);
void	hdoc_cleanup(t_redir *r);

#endif