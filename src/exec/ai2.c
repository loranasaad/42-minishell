/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you                                            +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "exec.h"
#include <unistd.h>      /* pipe, dup2, close, write */
#include <signal.h>      /* signal, SIGINT, SIGQUIT */
#include <sys/wait.h>    /* waitpid, WIFEXITED...   */
#include <errno.h>       /* errno                   >
#include <string.h>      /* ft_strchr (yours)       */

/* ---------- tiny local helpers (duplicated from exec_one style) ---------- */

static int  status_to_rc(int st)
{
    if (WIFEXITED(st))
        return (WEXITSTATUS(st));
    if (WIFSIGNALED(st))
        return (128 + WTERMSIG(st));
    return (1);
}

static void exec_error(const char *cmd)
{
    const char *p1 = "minishell: ";
    const char *p2 = ": command not found\n";

    write(2, p1, ft_strlen(p1));
    write(2, cmd, ft_strlen(cmd));
    write(2, p2, ft_strlen(p2));
}

/* Stub: replace when your builtin dispatcher is ready */
__attribute__((weak))
int builtin_dispatch(char **argv, t_ms *ms)
{
    (void)ms;
    if (!argv || !argv[0])
        return (-1);
    return (-1); /* not a builtin (stub) */
}

/* ---------- pipeline flattening: left-assoc tree → array [A,B,...,Z] ----- */

static int  count_pipeline(const t_ast *root)
{
    int n;

    if (!root)
        return (0);
    n = 0;
    while (root->kind == AST_PIPE)
    {
        n++;
        root = root->left;
    }
    return (n + 1); /* PIPE count + 1 = stages */
}

/* out must have room for n = count_pipeline(root) entries */
static void flatten_pipeline(const t_ast *root, t_ast **out, int n)
{
    int     i;
    t_ast   const *cur;

    i = n - 1;
    cur = root;
    while (cur->kind == AST_PIPE)
    {
        out[i] = cur->right; /* fill from end with right child */
        i--;
        cur = cur->left;     /* walk down the left spine */
    }
    out[0] = (t_ast *)cur;    /* leftmost CMD */
}

/* ---------- fd/pipe helpers ------------------------------------------------ */

static void close_all_pipes(int (*pipes)[2], int npipe)
{
    int i;

    if (!pipes)
        return;
    i = 0;
    while (i < npipe)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

static int  create_pipes(int (*pipes)[2], int npipe)
{
    int i;

    i = 0;
    while (i < npipe)
    {
        if (pipe(pipes[i]) < 0)
            return (0);
        i++;
    }
    return (1);
}

/* ---------- per-child work ------------------------------------------------- */

static void setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

static void setup_child_ends(int idx, int n, int (*pipes)[2])
{
    if (idx > 0)                 /* not first → read from previous pipe */
        dup2(pipes[idx - 1][0], STDIN_FILENO);
    if (idx < n - 1)             /* not last  → write to current pipe   */
        dup2(pipes[idx][1], STDOUT_FILENO);
    close_all_pipes(pipes, n - 1);
}

/* Build spec for this stage, apply redirs, run builtin or execve; never returns */
static void child_run_stage(t_ast *node, t_ms *ms, int idx, int n, int (*pipes)[2])
{
    t_cmdspec   spec;
    char      **envp;
    char       *full;

    setup_child_signals();
    setup_child_ends(idx, n, pipes);

    spec.argv = NULL;
    spec.redirs = NULL;
    if (!build_cmdspec_from_segment(node->start, node->end, &spec))
        exit(2);

    if (!apply_redirs(spec.redirs))
    {
        free_cmdspec(&spec);
        exit(1);
    }
    if (!spec.argv || !spec.argv[0])   /* stage of pure redirs (rare but valid) */
    {
        free_cmdspec(&spec);
        exit(0);
    }

    /* builtin-in-child (pipelines): fine that it doesn't persist */
    {
        int b_rc;

        b_rc = builtin_dispatch(spec.argv, ms);
        if (b_rc >= 0)
        {
            free_cmdspec(&spec);
            exit(b_rc);
        }
    }

    envp = env_to_envp(ms->env);
    if (!envp)
    {
        ms_perror("minishell", "env");
        free_cmdspec(&spec);
        exit(1);
    }

    if (ft_strchr(spec.argv[0], '/'))
    {
        execve(spec.argv[0], spec.argv, envp);
        if (errno == ENOENT) { free_cmdspec(&spec); exit(127); }
        free_cmdspec(&spec); exit(126);
    }
    full = find_in_path(spec.argv[0], ms->env);
    if (!full)
    {
        exec_error(spec.argv[0]);
        free_cmdspec(&spec);
        exit(127);
    }
    execve(full, spec.argv, envp);
    if (errno == ENOENT) { free_cmdspec(&spec); exit(127); }
    free_cmdspec(&spec); exit(126);
}

/* Wait all children; return the status code of the last one */
static int  wait_pipeline(pid_t *pids, int n)
{
    int i;
    int st;
    int rc;

    rc = 1;
    i = 0;
    while (i < n)
    {
        if (waitpid(pids[i], &st, 0) > 0)
        {
            if (i == n - 1)      /* last stage defines $? */
                rc = status_to_rc(st);
        }
        i++;
    }
    return (rc);
}

/* ---------- public entry point -------------------------------------------- */

int exec_pipeline(t_ast *root, t_ms *ms)
{
    int         n;
    t_ast     **stages;
    pid_t      *pids;
    int         (*pipes)[2];
    int         i;
	//no need
    // if (!root)
    //     return (0);

    n = count_pipeline(root);
    stages = (t_ast **)malloc(sizeof(t_ast *) * n);
    if (!stages)
        return (1);
    flatten_pipeline(root, stages, n);

    /* single stage should usually be handled by exec_one_cmd via exec_run_node,
       but handle gracefully if we ever get here: */
    // if (n == 1)
    // {
    //     t_cmdspec spec;
    //     int       rc;

    //     spec.argv = NULL;
    //     spec.redirs = NULL;
    //     if (!build_cmdspec_from_segment(stages[0]->start, stages[0]->end, &spec))
    //     {
    //         free(stages);
    //         return (2);
    //     }
    //     rc = exec_one_cmd(&spec, ms);
    //     free_cmdspec(&spec);
    //     free(stages);
    //     return (rc);
    // }

    pipes = NULL;
    if (n - 1 > 0)
    {
        pipes = (int (*)[2])malloc(sizeof(int[2]) * (n - 1));
        if (!pipes)
        {
            free(stages);
            return (1);
        }
        if (!create_pipes(pipes, n - 1))
        {
            free(pipes);
            free(stages);
            return (1);
        }
    }

    pids = (pid_t *)malloc(sizeof(pid_t) * n);
    if (!pids)
    {
        close_all_pipes(pipes, n - 1);
        free(pipes);
        free(stages);
        return (1);
    }

    i = 0;
    while (i < n)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            ms_perror("minishell", "fork");
            /* best effort: parent closes and waits already-started ones */
            close_all_pipes(pipes, n - 1);
            /* no full reap here; next loop can still wait, or you can add cleanup */
            free(pids);
            free(pipes);
            free(stages);
            return (1);
        }
        if (pids[i] == 0)
            child_run_stage(stages[i], ms, i, n, pipes);
        i++;
    }

    close_all_pipes(pipes, n - 1);
    {
        int rc;

        rc = wait_pipeline(pids, n);
        free(pids);
        free(pipes);
        free(stages);
        return (rc);
    }
}