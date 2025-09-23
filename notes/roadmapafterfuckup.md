Day 6 — Finish single-cmd exec & prep for pipes

Loran
	•	Finalize exec_one_cmd polish (consistent error prints via ms_perror, free full_path/envp where needed).
	•	Add a tiny exec_run_node(t_ast*, t_ms*) wrapper that:
	•	If AST_CMD → call exec_one_cmd.
	•	If AST_PIPE → will call exec_pipeline (stub for now: return 2 or “NYI”).
	•	REPL glue: set ms->last_status from exec_run_node.

Laura
	•	builtin_dispatch(char **argv, t_ms *ms) stub: recognize 7 names (echo pwd env cd export unset exit), return -1 if not builtin, 0 if it is (no action yet).
	•	Ensure build_cmdspec_from_segment always yields:
	•	argv NULL-terminated
	•	redirs list ordered as seen
	•	safe on empty argv (pure redirs)
	•	Tiny helper in include/parser.h: int is_builtin_name(const char *s); (used by dispatcher).

Day 7 — Pipelines (executor) + Expansion v1 (no field splitting)

Loran (Pipes)
	•	Implement int exec_pipeline(t_ast *root, t_ms *ms):
	•	Iterate stages (left-assoc): for each stage, create a pipe; fork child:
	•	Left child: dup2(pipe_out, STDOUT_FILENO); redirs; exec.
	•	Middle child(ren): both ends (dup2(prev_in, STDIN) & dup2(pipe_out, STDOUT)).
	•	Right child: dup2(prev_in, STDIN_FILENO); redirs; exec.
	•	Parent closes all unused fds immediately.
	•	waitpid all; ms->last_status = status of last stage.
	•	Signals: parent keeps prompt handlers; children set SIGINT/SIGQUIT to SIG_DFL.

Laura (Expansion v1)
	•	In cmdspec builder, strip quotes (remove ' and " characters).
	•	Implement $VAR and $? substitution:
	•	Apply to argv items and redir targets.
	•	Missing var → empty string.
	•	No field splitting yet (i.e., keep each token as a single field even if expansion inserts spaces).

Day 8 — Heredoc <<

Loran
	•	Implement heredoc reader:
	•	Read until delimiter (no history).
	•	If limiter unquoted → expand $; if quoted → no expansion.
	•	On Ctrl-C: clean up, return special status (130).
	•	Write to a temp file; return a path/FD to use as R_IN.
	•	Integrate in executor: prepare heredocs before forking the stage; convert R_HDOC → R_IN with the prepared FD.

Laura
	•	Make cmdspec accept heredoc “target” as limiter; mark whether it was quoted.
	•	Free/unlink helpers for heredoc temps.

Day 9 — Builtins

Loran
	•	Implement simple builtins: echo -n, pwd, env (pure output).
	•	builtin_dispatch returns an enum/rc; a run_builtin_child that executes in child (pipeline).

Laura
	•	Implement stateful builtins: cd, export, unset, exit.
	•	run_builtin_parent path when no pipeline (affects env / PWD).
	•	Integrate with dispatcher so REPL chooses parent vs child correctly.

Day 10 — Polish & Defense

Both
	•	Signals re-check (prompt vs child; heredoc Ctrl-C).
	•	Exit codes: 2 (syntax), 126/127 (exec), 130/131 (signals), builtin returns.
	•	Leak pass (tokens, AST, cmdspec, heredocs, env).
	•	Norm pass (≤25 lines, ≤5 locals, headers).
	•	Test sweep and defense notes (diagram + exit-code matrix).