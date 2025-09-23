Day 6 — Detailed To-Dos



Laura — CmdSpec builder + Builtin stub



Goal: produce a clean t_cmdspec (argv + redirs) from an AST_CMD slice so Loran can execute it as-is.

CmdSpec builder (build_cmdspec_from_segment)



Inputs: t_token *start, *end (half-open slice [start, end)), output t_cmdspec *out.

Init: set out->argv = NULL; out->redirs = NULL;.

First pass (count):

Walk tokens start..end:

Count argv_words = number of TK_WORD not consumed by redirs (i.e., words that are not the filename after a redir).

Count redir_count for TK_IN/TK_OUT/TK_APP/TK_HDOC.

Alloc:

Allocate out->argv as argv_words + 1 (NULL-terminated). Return 0 on alloc fail.

Second pass (fill):

Walk tokens again with an index ai for argv.

If TK_WORD and not the immediate target of a redir → out->argv[ai++] = ft_strdup(tok->val).

If TK_IN/TK_OUT/TK_APP/TK_HDOC:

Advance to tok->next (validator guarantees it’s a TK_WORD).

Allocate a t_redir node: set kind, target = ft_strdup(next->val), link at tail of out->redirs.

Skip the target token in iteration.

Terminate argv with NULL. Return 1 on success, 0 on any allocation failure (ensure partial frees on failure).



Free function (already done)



Quick sanity-check: it frees every argv[i], the argv array, each redir->target, and nodes; resets pointers to NULL.



Builtin dispatcher (skeleton)



File src/builtin/builtin.c.

Prototype: int builtin_dispatch(char **argv, t_ms *ms);

Behavior today:

If argv is NULL or argv[0] is NULL → return -1.

If argv[0] matches one of: echo, pwd, env, cd, export, unset, exit → return 0 (recognized, success stub).

Else return -1 (not a builtin).

No implementations yet; just recognition so executor can branch later.



Done checks for Laura



For grep hi > out.txt: argv = ["grep", "hi", NULL], redirs = [R_OUT "out.txt"].

For cat < in >> out: argv = ["cat", NULL], redirs = [R_IN "in", R_APP "out"].

No leaks on failure paths (simulate alloc fail by early returns and see frees get called).



Loran — PATH resolution + exec glue



Goal: make exec_one_cmd run external commands using Laura’s cmdspec; correct error codes.

Executor flow (single command)



Preconditions: you already call Laura’s builder in REPL; you already set child signals + apply_redirs.

In child after redirs:

If builtin_dispatch(argv, ms) >= 0: (stub for today)

exit(0);  // child path for now (parent-builtin optimization later)

Else external:

Build envp = env_to_envp(ms->env); on fail → print error → exit(1).



PATH resolution



If argv[0] contains ‘/’:

Try execve(argv[0], argv, envp).

On failure: map errno → exit code (see #4).

Else (no slash):

Get PATH with your env getter; if missing/empty → print minishell: <cmd>: command not found → exit(127).

Split by :; for each dir (empty entry = "."):

Build dir + "/" + cmd; if access(candidate, X_OK) == 0 → execve(candidate, argv, envp).

If none worked → print command not found → exit(127).



Parent wait + statuses



waitpid(pid, &st, 0).

If WIFEXITED → rc = WEXITSTATUS(st).

Else if WIFSIGNALED → rc = 128 + WTERMSIG(st) (→ 130 for SIGINT, 131 for SIGQUIT).

Return rc to REPL; set ms->last_status.



Exit-code policy (child side)



Redir/envp failure → exit(1).

command not found → exit(127).

Found but not executable / is directory / permission / bad format (EACCES, EISDIR, ENOEXEC) → exit(126).

For direct path (./cmd) that doesn’t exist (ENOENT) → exit(127).



Done checks for Loran



echo hi → prints hi, status 0.

idontexist → error msg, status 127.

./file (644, not exec) → status 126.

cat < nofile → redir error, status 1.

Ctrl-C while sleep 5 runs → child dies, shell prompt returns, status 130.

Hand-off points (very important)

Laura guarantees: cmdspec argv/targets are already quote-cleaned later (not today), and redir targets are strings you can open.

Loran consumes: spec->argv and spec->redirs as given; no extra parsing/expansion today.