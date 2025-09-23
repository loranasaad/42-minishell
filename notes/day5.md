Day 5 — Loran (A) Execution (single command, no pipes)

0) What you’ll build today
	•	Run a single AST_CMD (no AST_PIPE yet).
	•	Use Laura’s spec (t_cmdspec: argv[] + redirs) and env helpers.
	•	Correct parent vs child signal behavior (bash-like).
	•	Set ms->last_status from the child’s result.

1) Headers & prototypes to rely on

Make sure these are visible (they can live in parser.h / minishell.h / env.h):
	•	From Laura:
	•	int  build_cmdspec_from_segment(t_token *start, t_token *end, t_cmdspec *out);
	•	void free_cmdspec(t_cmdspec *spec);
	•	char **env_to_envp(t_env *env);
	•	void   free_envp(char **envp);
	•	char  *find_in_path(const char *name, t_env *env);
	•	Your exec API (new, add to include/exec.h):
	•	int apply_redirs(const t_redir *rlist);          // 1 ok, 0 err
	•	int exec_one_cmd(const t_cmdspec *spec, t_ms *ms); // return exit status

Include order in .c files where you run:
#include "minishell.h", #include "parser.h", #include "exec.h", plus system headers (unistd.h, fcntl.h, sys/wait.h, signal.h, errno.h, string.h).

⸻

2) REPL glue (only when root is AST_CMD)

In your REPL after parse success:
	1.	If root->kind == AST_PIPE → skip for Day 5 (we’ll do Day 6).
	2.	If root->kind == AST_CMD:
	•	Declare t_cmdspec spec; and zero-init its fields.
	•	Call build_cmdspec_from_segment(root->start, root->end, &spec).
	•	On failure: set ms->last_status = 2; continue loop.
	•	Call ms->last_status = exec_one_cmd(&spec, ms);
	•	Call free_cmdspec(&spec);

Keep your existing frees: ast_free(ast); free_tokens(toks);

⸻

3) apply_redirs (child-side, no pipes)

Create src/exec/redir.c.

Behavior
	•	Iterate the t_redir list (may be NULL).
	•	For each node:
	•	Map kind → open flags:
	•	R_IN:  open(target, O_RDONLY)
	•	R_OUT: open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644)
	•	R_APP: open(target, O_WRONLY | O_CREAT | O_APPEND, 0644)
	•	R_HDOC: Day 5: not implemented. For today: return 0 after printing a friendly “heredoc not implemented” error. (We’ll implement on Day 6.)
	•	On success:
	•	For R_IN: dup2(fd, STDIN_FILENO) then close(fd)
	•	For R_OUT / R_APP: dup2(fd, STDOUT_FILENO) then close(fd)
	•	On any failure:
	•	Print an error (e.g., minishell: <file>: <strerror(errno)>)
	•	Return 0
	•	If all succeeded: return 1

Keep it simple; you’ll reuse this for pipelines later.

⸻

4) exec_one_cmd (single command, no pipes)

Create src/exec/exec_one.c.

Outline (no full code)
	1.	Empty argv handling
	•	If spec->argv == NULL or spec->argv[0] == NULL:
	•	If there are no redirs: return 0 (nothing to run).
	•	If there are redirs:
	•	fork(); in the child: apply_redirs(...) then _exit(0)
	•	parent: waitpid, translate status → return it.
	2.	Resolve program path
	•	Ask Laura’s find_in_path(spec->argv[0], ms->env).
	•	If returns NULL:
	•	Print: minishell: <cmd>: command not found
	•	Return 127
	3.	Build envp
	•	char **envp = env_to_envp(ms->env);
	•	If alloc fails → print error, free(full), return 1 (generic)
	4.	Fork
	•	pid = fork();
	•	If fork fails: print error, free full + envp, return 1
	5.	Child process
	•	Restore default signals:
	•	signal(SIGINT, SIG_DFL);
	•	signal(SIGQUIT, SIG_DFL);
	•	Call apply_redirs(spec->redirs).
	•	If it returns 0: _exit(1) (redir error)
	•	execve(full, spec->argv, envp);
	•	If it fails:
	•	If errno == EACCES: _exit(126)  (permission)
	•	Else: _exit(127)                  (not found / no exec)
	•	(Do not print extra stuff after execve fails; decide one error path)
	6.	Parent process
	•	waitpid(pid, &st, 0);
	•	If WIFEXITED(st):
	•	rc = WEXITSTATUS(st) (0..255)
	•	Else if WIFSIGNALED(st):
	•	sig = WTERMSIG(st)
	•	If sig == SIGINT: you may print a newline; set rc = 128 + sig (→ 130)
	•	If sig == SIGQUIT: optionally print “Quit (core dumped)”; rc = 128 + sig (→ 131)
	•	Free full and envp
	•	Return rc

Parent keeps your prompt handlers; child uses default so Ctrl-C kills the child (as in bash).

⸻

5) Error message policy (be consistent)
	•	Not found (no path hit): minishell: <cmd>: command not found → 127
	•	Permission denied: minishell: <path>: Permission denied → 126
	•	Redirection open errors: minishell: <file>: <strerror(errno)> → child _exit(1)
	•	Heredoc NYI (today): print minishell: heredoc: not implemented → return error (choose: either fail before fork with 2, or let child _exit(1)). Pick one and keep it.

⸻

6) REPL signal expectations (sanity)
	•	Parent: your handlers as before (Ctrl-C makes a fresh prompt, Ctrl-\ ignored).
	•	Child: defaults — Ctrl-C interrupts the child; parent collects 130.

⸻

7) Minimal tests to run today

Run these from your minishell:
	1.	/bin/echo hello → prints hello, returns 0
	2.	idontexist → 127 and error message
	3.	./some-non-exec (chmod 644) → 126
	4.	cat < in.txt > out.txt → copies contents (single process)
	5.	echo hi | wc → skip (pipeline is Day 6); ensure it doesn’t crash and leaves status from parser (or just prints “pipelines NYI” if you add a placeholder)
	6.	Press Ctrl-C while sleep 5 is running → child dies, parent returns to prompt, status 130
	7.	Press Ctrl-\ while sleep 5 is running → status 131 (optional message)


