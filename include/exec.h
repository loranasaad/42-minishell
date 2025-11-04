/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: latabagl <latabagl@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:17:31 by loasaad           #+#    #+#             */
/*   Updated: 2025/11/04 17:21:01 by latabagl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "parser.h"
# include <signal.h>

# define CU_HDOCS (1 << 0)
# define CU_SPECS (1 << 1)
# define CU_STAGES (1 << 2)
# define CU_PIPES (1 << 3)
# define CU_PIDS (1 << 4)
# define CU_CLOSE_PIPES (1 << 5)

# define CU_ALL (CU_HDOCS | CU_SPECS | CU_STAGES | CU_PIPES \
	| CU_PIDS | CU_CLOSE_PIPES)
# define CU_BASIC (CU_HDOCS | CU_SPECS | CU_STAGES)
# define CU_FULL_PIPES (CU_HDOCS | CU_SPECS \
	| CU_STAGES | CU_PIPES | CU_CLOSE_PIPES)
# define CU_SUCCESS (CU_HDOCS | CU_SPECS | CU_STAGES | CU_PIPES | CU_PIDS)

typedef struct s_hdoc_prep
{
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;
	int					(*old_event_hook)(void);

}						t_hdoc_prep;

typedef struct s_child_args
{
	t_cmdspec	*spec;
	t_ms		*ms;
	t_cu		*cleanup;
}				t_child_args;

typedef struct s_pipe_data
{
	t_ast		**stages;
	t_cmdspec	*specs;
	pid_t		*pids;
	int			(*pipes)[2];
	int			len;
}				t_pipe_data;

typedef struct s_sig_backup
{
	struct sigaction	old_sigint;
	struct sigaction	old_sigquit;
	int					(*old_event_hook)(void);
}						t_sig_backup;

//exec_child_utils
void	prep_cleanup(t_pipe_data *data, t_cu *cu, int i);
//exec_child
void	set_child_pipes(int i, t_pipe_data *data, t_ms *ms, t_cu *c_up);
void	exec_child_command(t_cmdspec *spec, t_ms *ms, t_cu *cleanup);
//exec_node
int		exec_run_node(t_ast *node, t_ms *ms, t_cu *cleanup);
//exec_one
int		exec_one_cmd(t_cmdspec *spec, t_ms *ms, t_cu *cleanup);
// exec_path
void	exec_path_search(t_cmdspec *spec, char **envp, t_ms *ms, t_cu *cleanup);
void	exec_direct_path(t_cmdspec *spec, char **envp, t_ms *ms, t_cu *cleanup);
//exec_pipeline_setup
int		make_pipes(int (*pipes)[2], int len);
int		count_pipeline(t_ast *root);
void	flatten_pipeline(t_ast *root, t_ast **commands, int length);
int		build_all_specs(t_ast **stages, int len, t_cmdspec *specs, t_ms *ms);
int		prepare_all_heredocs(t_cmdspec *specs, int len, t_ms *ms);
//exec_pipeline_cleanup
void	hdoc_cleanup_specs(t_cmdspec *s, int len);
void	free_specs(t_cmdspec *specs, int len);
void	close_pipes(int (*pipes)[2], int len);
void	pipeline_cleanup(t_pipe_data *data, int flags);
void	hdoc_cleanup(t_redir *r);
//exec_pipeline
int		exec_pipeline(t_ast *root, t_ms *ms, t_cu *cleanup);
//exec_stateful
int		exec_stateful(t_cmdspec *spec, t_ms *ms);
//exec_utils
void	child_cleanup_all(t_ms *ms, t_cu *cleanup);
int		status_to_rc(int st);
void	exec_error(const char *cmd);
int		is_stateful(const char *name);
int		is_builtin(const char *name);
//hdoc_fill
int		hdoc_fill(int wfd, char *limiter, int expand, t_ms *ms);
//hdoc_prep
int		hdoc_prepare(t_redir *redirs, t_ms *ms);
//hdoc_signal
int		hdoc_event(void);
void	hdoc_sigint_handler(int sig);
void	hdoc_restore_sig(struct sigaction *oldi, struct sigaction *oldq);
void	hdoc_init_sig(struct sigaction *oldi, struct sigaction *oldq);
//redir
int		apply_redirs(t_redir *rlist);

#endif