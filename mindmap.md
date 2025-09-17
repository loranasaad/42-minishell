```mermaid
flowchart TD

%% ==== ENTRY & STATE ====
M["main: init env, signals, t_ms"] --> R["repl()"]

%% ==== REPL LOOP ====
R --> RL["readline(prompt)"]
RL -->|NULL (Ctrl-D)| EXITD["print 'exit' then return"]
RL -->|non-empty| AH["add_history(line)"]
RL -->|non-empty| LEX["lex(line) -> tokens"]

%% ==== FRONTEND -> CORE ====
LEX --> PAR["parse(tokens) -> AST"]
PAR --> EXP["expand(AST, ms)"]
EXP --> HD["heredoc_prepare(AST, ms)"]
HD  --> EXE["exec_ast(AST, ms)"]

%% ==== EXECUTION PATHS ====
EXE --> DEC{"builtin?"}
DEC -->|yes & no pipe| BPAR["run_builtin_parent()"]
DEC -->|yes & in pipe| BCHILD["run_builtin_child()"]
DEC -->|no (external)| PATHR["path_resolve(argv0, $PATH)"]
PATHR --> ENV2["env_to_envp(env)"]
ENV2  --> EXECVE["execve()"]

%% ==== PARENT / STATUS ====
EXE --> PWAIT["parent_wait_last()"]
PWAIT --> STATUS["ms.last_status  (= $?)"]