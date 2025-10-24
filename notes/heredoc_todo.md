great — let’s build heredoc cleanly, step by step, from zero. no code yet, just precise tasks + why each exists so you can implement without surprises.

⸻

Step 0 — Decide the data you need (tiny struct tweak)

What to add to your t_redir:
	•	int hdoc_fd; — read-end FD you’ll feed to the command’s stdin.
	•	int hdoc_expand; — 1 if delimiter unquoted (do $ expansion in the body), 0 if quoted (no expansion).

Why: a heredoc is not a filename. You capture user input into a pipe (or temp file) and then redirect stdin from that FD. The hdoc_expand flag lets you honor bash’s rule: unquoted delimiter → expand, quoted → don’t.

Where set: in Laura’s builder when she strips quotes from the delimiter token:
	•	If the original limiter token was quoted → hdoc_expand = 0.
	•	If not quoted → hdoc_expand = 1.
	•	Keep target as the raw limiter string (without quotes) for comparison.

⸻

Step 1 — Add a preparation pass (before any fork/exec)

New function (name your way, e.g.):
	•	int heredoc_prepare_cmd(t_cmdspec *spec, t_ms *ms);
Return 1 on success, 130 if user hit Ctrl-C during capture (so caller aborts), or 0/1 for other errors (pick one policy and be consistent).

What it does:
	•	Walk spec->redirs.
	•	For every R_HDOC:
	•	Create a pipe() for this heredoc.
	•	Fork a tiny “heredoc helper” child to read lines.
	•	Parent waitpid() that child and converts the result:
	•	If child was killed by SIGINT → return 130 (cancel the whole command line).
	•	If exited 0 → store the read end in r->hdoc_fd and close the write end in the parent.
	•	On other failures → print an error and choose a return (1 is fine).

Why a separate pass: you want all heredocs captured before you start forking the real pipeline, so:
	•	You can cancel everything on Ctrl-C cleanly.
	•	You avoid interleaving interactive prompts with N children and pipes.

⸻

Step 2 — The heredoc helper child (signal model + loop)

Child setup:
	•	SIGINT → default (so Ctrl-C interrupts this child).
	•	SIGQUIT → ignored.

Loop:
	•	Repeatedly readline("> ").
	•	Stop if:
	•	The line is NULL (Ctrl-D / EOF), or
	•	The line equals the limiter (string compare).
	•	Otherwise:
	•	If hdoc_expand == 1, run expansion on the line ($VAR, $?) using the same expander Laura is writing (no field splitting).
	•	Write the resulting line + \n to the pipe write end.

Exit:
	•	Close write end.
	•	exit(0) on normal termination.
	•	If interrupted by Ctrl-C, the process receives SIGINT and the parent will see it; you don’t need extra logic.

Why these signals: matches bash behavior:
	•	Ctrl-C cancels heredoc capture (not the whole shell).
	•	Ctrl-\ is ignored.

⸻

Step 3 — Parent behavior during heredoc capture

Before waitpid:
	•	Temporarily ignore SIGINT in the parent (so your shell itself isn’t killed while the child is reading).
	•	After waitpid, restore your prompt handler.

After waitpid:
	•	If WIFSIGNALED with SIGINT → abort: set ms->last_status = 130, close any open pipe ends, return 130 to caller (so REPL drops the command and prints a fresh prompt).
	•	Else success:
	•	Close the write end.
	•	Keep the read end in r->hdoc_fd.

Why ignore SIGINT in parent: you want Ctrl-C to hit the heredoc child, not your shell process.

⸻

Step 4 — Teach apply_redirs how to use heredoc FDs

Current behavior: for <, >, >> you open() and dup2().

Add for heredoc:
	•	If r->kind == R_HDOC:
	•	dup2(r->hdoc_fd, STDIN_FILENO);
	•	close(r->hdoc_fd);
	•	Do not call open() — you already have the FD.

Alternative: you can mutate R_HDOC → R_IN after prepare, and set target = NULL plus hdoc_fd >= 0, then in apply_redirs:
	•	If an input redir has hdoc_fd >= 0, use that; otherwise open(target, O_RDONLY).

Why: keeps apply_redirs generic and under your 25-line limit (move per-redir logic to tiny helpers if needed).

⸻

Step 5 — Integrate in single-cmd and pipelines (call sites)

Single command path (exec_one_cmd):
	•	Before forking:
	•	Call heredoc_prepare_cmd(&spec, ms).
	•	If it returns 130 → set ms->last_status = 130 and return (don’t fork).
	•	If it returns 0 (your error code) → set a generic error (1) and return.
	•	Then proceed to fork/exec as you already do. The child will call apply_redirs, which will see the heredoc FDs.

Pipeline path (exec_pipeline):
	•	After you flatten into commands[], but before creating pipes/forking:
	•	For each stage i, build t_cmdspec (as you already do for children) only for the purpose of heredoc prepare, or run a lighter “scan the tokens for heredocs” helper if you have one.
	•	Call heredoc_prepare_cmd(&spec, ms) per stage.
	•	If any returns 130 → close anything opened so far and abort pipeline with rc=130.
	•	If any error → abort with rc=1.
	•	Store the resulting hdoc_fd in the redir list carried to exec_child.

Why before forking: ensures user interaction (readline) happens once, cleanly, and your pipeline logic stays purely about FDs.

⸻

Step 6 — Expansion inside heredoc lines (when allowed)
	•	If hdoc_expand == 1: run your variable expander on each line during capture.
	•	Expand $VAR using env_get.
	•	Expand $? using ms->last_status.
	•	No field splitting and no globbing here.
	•	If hdoc_expand == 0: write the line as-is.

Why here: bash expands heredoc content at capture time only when the delimiter is unquoted.

⸻

Step 7 — Cleanup responsibilities
	•	Child (real exec child): after dup2(hdoc_fd, STDIN), close that FD.
	•	Parent: after forking children for exec/pipeline, the parent should close all heredoc read FDs it still holds (they were passed down to children already). Otherwise you can keep pipes open accidentally and block EOF.
	•	On abort (Ctrl-C during capture): close both ends immediately and free any partially built state.

⸻

Step 8 — Minimal tests (once you wire it)
	1.	cat <<EOF → type a couple lines → EOF → the lines echo through cat.
	2.	Quoted limiter: cat <<'EOF' → $USER inside must not expand.
	3.	Unquoted limiter: cat <<EOF → $USER must expand.
	4.	Ctrl-C in heredoc prompt → return to minishell prompt, $? = 130; nothing runs.
	5.	Pipeline: cat <<EOF | wc -l → heredoc feeds the first stage, pipeline continues.

⸻

If you want next, I can map exactly where to place the four tiny building blocks in your tree:
	•	t_redir additions,
	•	the heredoc_prepare_cmd outline,
	•	the helper child loop logic (with signals),
	•	and the one-line change in apply_redirs for R_HDOC.