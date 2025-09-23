# 🐚 Minishell 10-Day Roadmap

> Roles  
> **Loran (A):** REPL + signals, lexer, expansion, heredoc, env helpers, builtins (echo/pwd/env)  
> **Laura (B):** executor (fork/exec/dup2/pipe), redirs, PATH resolution, statuses, builtins (cd/export/unset/exit)  
> **Shared:** parser/AST, memory cleanup, test suite, polish

---

## Day 1 — Kickoff & Setup
- [ ] Agree on repo structure (`include/`, `src/`, `Makefile`)
- [ ] Draft core structs: tokens, AST, redirs, cmd, env
- [ ] Makefile skeleton with mandatory rules (`NAME`, `all`, `clean`, `fclean`, `re`)
- [ ] Stub `minishell` runs and exits on EOF

---

## Day 2 — REPL & Signals
- **Loran:**  
  - [ ] `readline` prompt + `add_history` (non-empty lines)  
  - [ ] Interactive detection; Ctrl-D exits  
  - [ ] One global int for received signal only  
  - [ ] Ctrl-C → newline + fresh prompt; Ctrl-\ ignored  
- **Laura:**  
  - [ ] Start environment list from `envp` (get/set/unset/to_envp)  

---

## Day 3 — Lexer v1
- **Loran:**  
  - [ ] Tokenize WORD, `|`, `<`, `>`, `>>`, `<<`  
  - [ ] Quote handling: `'` blocks all, `"` allows `$`  
  - [ ] Detect unclosed quotes → syntax error  
- **Laura:**  
  - [ ] Token printer/debugger for testing  

---

## Day 4 — Parser → AST
- **Shared:**  
  - [ ] Grammar: `pipeline := command ('|' command)*`  
  - [ ] Attach redirs to closest command  
  - [ ] Syntax errors: `|` at ends, missing filename after redir  
  - [ ] AST dumper for debugging  

---

## Day 5 — Executor Skeleton
- **Laura:**  
  - [ ] Implement N-stage pipeline (`pipe`, `fork`, `dup2`)  
  - [ ] Parent closes fds, waits, stores status from last cmd  
  - [ ] Child restores default signals  
- **Loran:**  
  - [ ] Ensure parent vs child signal handling matches bash  

---

## Day 6 — PATH & Expansion v1
- **Laura:**  
  - [ ] PATH search; handle 127 (not found) / 126 (permission/dir)  
- **Loran:**  
  - [ ] Quote removal + variable expansion (`$VAR`, `$?`)  
  - [ ] Field splitting on unquoted spaces  
  - [ ] Apply expansion to argv and redir targets  

---

## Day 7 — Redirections
- **Laura:**  
  - [ ] Implement `<`, `>`, `>>` with correct open modes  
  - [ ] Handle errors → print + exit child with code  
- **Loran:**  
  - [ ] Write tests mixing redirs & pipes  

---

## Day 8 — Heredoc `<<`
- **Loran:**  
  - [ ] Read until delimiter; quoted = no expansion, unquoted = expand `$`  
  - [ ] Ctrl-C cancels heredoc (status 130), cleanup temp  
  - [ ] No history updates during heredoc  
- **Laura:**  
  - [ ] Integrate heredoc into exec (prepare before fork, unlink temps)  

---

## Day 9 — Builtins
- **Loran:**  
  - [ ] Implement `echo -n`, `pwd`, `env`  
- **Laura:**  
  - [ ] Implement `cd`, `export`, `unset`, `exit`  
- **Shared:**  
  - [ ] Dispatcher: `is_builtin` / `run_builtin`  
  - [ ] Run in parent when no pipeline (stateful builtins)  

---

## Day 10 — Polish & Defense Prep
- [ ] Signals: re-verify prompt vs child behaviors (Ctrl-C, Ctrl-D, Ctrl-\)  
- [ ] Exit codes: 127, 126, 2 (syntax), 130/131 for signals  
- [ ] Free everything (tokens, AST, env, argv, redirs, heredoc temps)  
- [ ] Norm compliance check  
- [ ] Full test suite (syntax, expansion, pipes, redirs, heredocs, builtins, signals)  
- [ ] Compare with bash for unclear behaviors  
- [ ] Defense sheet: architecture diagram, exit code matrix, heredoc rules, global var rationale  

---

# ✅ Definition of Done
- Prompt + history  
- Pipes `|`  
- Redirs `<`, `>`, `>>`, `<<` (with heredoc rules)  
- Expansion: `$VAR`, `$?`, quotes, field splitting  
- Builtins: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`  
- Signals: Ctrl-C/-D/-\ per bash spec  
- One global int for signal only  
- No leaks from our code (readline leaks tolerated)  

---