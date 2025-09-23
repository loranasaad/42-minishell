Day 6 — Pipelines (do now)

Loran
	•	exec_pipeline(t_ast *root, t_ms *ms):
	•	Traverse left→right stages.
	•	For each stage: build t_cmdspec, make pipe (except last), fork.
	•	Child: set default signals, wire stdin/stdout via dup2, apply_redirs, resolve+execve.
	•	Parent: close write-end immediately, carry read-end to next stage, waitpid all at end.
	•	Set ms->last_status from last stage (map signals → 128+sig).
	•	REPL: route AST_PIPE → exec_pipeline, else exec_one_cmd.

Laura
	•	Harden build_cmdspec_from_segment for:
	•	pure redirs (argv empty)
	•	mixed order of words/redirs
	•	Confirm find_in_path() behavior: empty PATH entry = “.”, missing PATH, names with “/”.
	•	Ensure env_to_envp + free_envp robust.

Done checks
	•	echo hi | wc -c
	•	cat < in | grep foo > out
	•	idontexist | wc → 127 (from left) but $? from last stage
	•	Ctrl-C during sleep 5 | cat → pipeline dies, parent prompt returns, $? 130

⸻

Day 7 — Expansion v1 (no field splitting)

Laura
	•	In cmdspec builder (only here—not the lexer):
	•	Quote removal in WORDs.
	•	Variable expansion $VAR from env; $? from ms->last_status.
	•	Apply to both argv[] and redir targets.
	•	Keep each expanded token as one field (no splitting yet).

Loran
	•	Call the expanded builder everywhere you already build cmdspecs (single + pipeline).
	•	Quick regression tests:
	•	echo "$USER" (quoted keeps spaces if any)
	•	echo $? after failing cmd → shows correct last status
	•	Missing var → empty string (not removed)

Done checks
	•	echo "$USER X" prints exactly two fields in argv.
	•	cat "$FILE" tries literal expanded path.

⸻

Day 8 — Heredoc <<

Loran
	•	Heredoc reader:
	•	Read until delimiter.
	•	Quoted delimiter → no expansion; unquoted → expand $.
	•	Ctrl-C cancels heredoc (return status 130), clean temp.
	•	No history updates while reading.
	•	Produce a temp file/FD for the content.

Laura
	•	Integrate heredoc into exec:
	•	Pre-resolve all heredocs in a pipeline before forking that stage.
	•	Replace R_HDOC with an input FD in the cmdspec; unlink temps after fork/dup.

Done checks
	•	cat <<EOF then input → shows content
	•	cat << 'EOF' means no expansion
	•	Ctrl-C during heredoc returns to prompt, $? = 130

⸻

Day 9 — Builtins

Loran
	•	echo -n, pwd, env (stateless; can run in child or parent).
	•	Implement is_builtin(cmd) and run_builtin_child() for pipeline cases.

Laura
	•	cd, export, unset, exit (stateful; must run in parent when not in a pipeline).
	•	Dispatcher:
	•	If single command and parent-builtin → run in parent, no fork.
	•	Else (pipeline or non-stateful) → run in child.

Done checks
	•	cd /; pwd changes shell cwd
	•	export FOO=bar; env | grep FOO
	•	echo hi | wc still works
	•	exit 42 exits minishell with status 42

⸻

Day 10 — Polish & Defense

Both
	•	Signals matrix: prompt vs child (Ctrl-C/D/\) match bash.
	•	Exit codes: 2 (syntax), 126/127 (exec), 130/131 (signals), builtin returns.
	•	Memory: free tokens, AST, cmdspec (argv/redirs), heredoc temps, env; tolerate readline leaks.
	•	Norm pass (≤25 lines, ≤5 locals, headers), no ternaries (your rule).
	•	Test sweep: pipes + redirs + heredocs + expansion + builtins + statuses.
	•	Defense pack: tiny arch diagram + data flow + exit-code table + heredoc rules + global-int rationale.

⸻

Hand-off checkpoints (so you don’t block each other)
	•	End Day 6: exec_pipeline + hardened cmdspec.
	•	End Day 7: expanded cmdspec builder stable; executor already calls it.
	•	End Day 8: heredoc reader + integration; cmdspec carries input FD.
	•	End Day 9: builtin dispatch works parent vs child.

