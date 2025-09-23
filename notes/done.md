# ✅ Minishell Progress Checklist (up to Day 6)

This list captures everything completed so far, so you can safely start a new thread without losing context.

---

## Day 1 — Kickoff & Setup

* [x] Repo structure with `include/`, `src/`, `Makefile`
* [x] Core structs drafted: tokens, env, ms, AST (basic)
* [x] Makefile compiles with readline on Linux/macOS
* [x] Stub `minishell` runs and exits on EOF

---

## Day 2 — REPL & Signals

* [x] REPL loop with `readline`, `add_history`
* [x] Ctrl-D exits cleanly
* [x] Global `g_signal` for signal state
* [x] Ctrl-C → newline + fresh prompt (no `^C`)
* [x] Ctrl-\ ignored
* [x] Parent prompt vs child signals: separated handling with `rl_catch_signals = 0`

---

## Day 3 — Lexer v1

* [x] Tokenize WORD, `|`, `<`, `>`, `>>`, `<<`
* [x] Quote handling: `'` blocks all, `"` allows `$`
* [x] Detect unclosed quotes → syntax error (status 2)
* [x] Token helpers: `tok_new`, `tok_push_back`, `free_tokens`
* [x] Helpers: `is_space`, `is_meta`, `match_2`

---

## Day 4 — Parser → AST

* [x] Grammar: `pipeline := command ('|' command)*`
* [x] Syntax validator: detects bad `|`, missing redir targets
* [x] AST structs: `AST_CMD`, `AST_PIPE` with `left/right/start/end`
* [x] Parser wrapper `parse_line()`
* [x] `ast_free()` implemented
* [x] REPL integration: lex → parse → free

---

## Day 5 — Executor Skeleton (single command)

* [x] Redirection handling (`apply_redirs`) with helper `do_redir`
* [x] Error printing via `ms_perror`
* [x] `exec_one_cmd` implemented:

  * Forks child
  * Child resets signals → applies redirs → runs command via `execve`
  * PATH resolution with `find_in_path`
  * Correct exit codes: 0, 126, 127, 130, 131
  * Parent waits, converts status → `ms->last_status`
* [x] Pure redirs (no argv) supported
* [x] REPL updated to call `exec_one_cmd`

---

## Day 6 — Exec glue + PATH finalize

* [x] `exec_run_node(t_ast*, t_ms*)` stub implemented
* [x] Wired into REPL (replaces manual AST\_CMD/Pipe handling)
* [x] Placeholder `exec_pipeline.c` created, returns `2` (NYI)
* [x] PATH search finalized (`find_in_path`) with current dir support (`:` → `.`)
* [x] Error messages for `command not found`
* [x] Status code mapping verified

---

## Not Yet Done

* [ ] Pipeline execution (`exec_pipeline`)
* [ ] Expansion: quote removal, `$VAR`, `$?`, field splitting
* [ ] Heredoc `<<`
* [ ] Builtins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
* [ ] Final polish: defense prep, signal matrix, leaks, norm check
