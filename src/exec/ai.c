/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:26:43 by loasaad           #+#    #+#             */
/*   Updated: 2025/10/20 13:10:21 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "minishell.h"
#include "parser.h"
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>     // ADDED: for dup(), dup2(), close()

/* ---------- small helpers you already have elsewhere ---------- */
/* Keep these prototypes if helpers live in another file. */
// int   status_to_rc(int st);
// void  exec_error(const char *cmd);
// int   is_builtin(const char *name);
// void  builtin_dispatch(char **argv, t_ms *ms, int *rc);

/* ---------- ADDED: save/restore std fds for parent-builtins ---- */
static int save_std(int saved[2])
{
    saved[0] = dup(STDIN_FILENO);
    if (saved[0] < 0)
        return (0);
    saved[1] = dup(STDOUT_FILENO);
    if (saved[1] < 0)
    {
        close(saved[0]);
        return (0);
    }
    return (1);
}

static void restore_std(int saved[2])
{
    if (saved[0] >= 0)
    {
        dup2(saved[0], STDIN_FILENO);
        close(saved[0]);
    }
    if (saved[1] >= 0)
    {
        dup2(saved[1], STDOUT_FILENO);
        close(saved[1]);
    }
}

/* ---------- ADDED: run a stateful builtin in the PARENT ---------- */
/* Applies redirs in parent, runs builtin, then restores stdio.      */
static int run_stateful_in_parent(const t_cmdspec *spec, t_ms *ms)
{
    int saved[2];
    int rc;

    if (!save_std(saved))
        return (1);
    if (!apply_redirs(spec->redirs))          // reuse your redir code
    {
        restore_std(saved);
        return (1);
    }
    rc = 0;
    builtin_dispatch(spec->argv, ms, &rc);    // run builtin in parent
    restore_std(saved);
    return (rc);
}

/* ---------- main entry ---------- */
int	exec_one_cmd(const t_cmdspec *spec, t_ms *ms)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*full_path;
    int     rc;                 // ADDED: reuse variable for builtin rc

	/* no argv + no redirs -> nothing to do */
	if ((!spec->argv || !spec->argv[0]) && !spec->redirs)
		return (0);

    /* ---------- ADDED: parent path for stateful builtins (no pipeline) ----
     * If the command is a stateful builtin (cd/export/unset/exit),
     * we must run it in the *parent* so its effects persist.
     * We also honor redirections by temporarily applying them in parent.
     */
    if (spec->argv && spec->argv[0] && is_stateful(spec->argv[0]))
    {
        return (run_stateful_in_parent(spec, ms));
    }

	/* normal path: fork a child (covers non-stateful builtins + externals) */
	pid = fork();
	if (pid < 0)
	{
		ms_perror("minishell", "fork");
		return (1);
	}
	if (pid == 0)
	{
		/* child */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		if (!apply_redirs(spec->redirs))
			exit(1);

		/* pure redirections (no command): success after applying redirs */
		if (!spec->argv || !spec->argv[0])
			exit(0);

        /* ---------- ADDED: run builtins in the child (non-stateful path) ----
         * Non-stateful builtins (echo/pwd/env) are fine in a child.
         * Stateful ones would have been handled above in the parent path.
         */
        if (is_builtin(spec->argv[0]))
        {
            rc = 0;
            builtin_dispatch(spec->argv, ms, &rc);
            exit(rc);
        }

		/* external command */
		envp = env_to_envp(ms->env);
		if (!envp)
		{
			ms_perror("minishell", "env");
			exit(1);
		}
		if (ft_strchr(spec->argv[0], '/'))              /* direct path */
		{
			execve(spec->argv[0], spec->argv, envp);
            /* execve failed */
            if (errno == ENOENT)
            {
                free_envp(envp);                        // ADDED: free before exit
			    exit(127);
            }
            free_envp(envp);                            // ADDED
			exit(126);
		}
		full_path = find_in_path(spec->argv[0], ms->env);
		if (!full_path)
		{
			exec_error(spec->argv[0]);
            free_envp(envp);                            // ADDED
			exit(127);
		}
		execve(full_path, spec->argv, envp);
        /* execve failed */
        if (errno == ENOENT)
        {
            free(full_path);                            // ADDED
            free_envp(envp);                            // ADDED
			exit(127);
        }
        free(full_path);                                // ADDED
        free_envp(envp);                                // ADDED
		exit(126);
	}
	/* parent */
	if (waitpid(pid, &status, 0) < 0)
	{
		ms_perror("minishell", "waitpid");
		return (1);
	}
	return (status_to_rc(status));
}