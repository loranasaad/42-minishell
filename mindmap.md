```mermaid
flowchart TD

%% ===================== ENTRY & STATE =====================
M["main.c: main(argc, argv, envp)"]
IE["env/init_env(envp) → t_env*"]
SS["signals/setup_interactive_handlers()"]
R["repl.c: repl(t_ms*)"]

MS[/"t_ms (env, $?, interactive)"/]

M --> IE
M --> SS
M --> R
IE -. updates .-> MS
SS -. config .-> MS

%% ===================== REPL LOOP =====================
RL["readline(prompt)"]
AH["add_history(line)"]
EXITD["print 'exit' and return"]

LINE[/"line: string"/]

R --> RL
RL -->|non-null| LINE
RL -->|NULL (Ctrl-D)| EXITD
LINE -->|non-empty| AH

%% ===================== FRONTEND → CORE =====================
LEX["lexer/lex(line)"]
TOK[/"tokens: t_token*"/]
PAR["parser/parse(tokens)"]
AST[/"AST: t_ast*"/]
EXP["expander/expand(AST, t_ms*)"]
XAST[/"expanded AST"/]
HD["heredoc/prepare(AST, t_ms*)"]
PLAN[/"ready to exec (fds prepped)"/]

LINE --> LEX --> TOK --> PAR --> AST --> EXP --> XAST --> HD --> PLAN

%% ===================== EXECUTION =====================
EXE["exec/exec_ast(AST, t_ms*)"]
PLAN --> EXE

F["fork children / build pipes"]
EXE --> F
CSET["child: dup2 pipes + redirs"]
F --> CSET

DEC{"builtin?"}
CSET --> DEC

BPAR["run builtin in parent (no pipe)"]
BCHILD["run builtin in child (in pipeline)"]
PATHR["exec/path_resolve(argv[0], $PATH)"]
ENV2["env/env_to_envp(t_env*) → char**"]
EXECVE["execve(binary, argv, envp)"]

DEC -->|single cmd| BPAR
DEC -->|in pipeline| BCHILD
DEC -->|no (external)| PATHR
PATHR --> ENV2 --> EXECVE

PWAIT["parent: close fds + wait last"]
STATUS[/"$?: last_status"/]
EXE --> PWAIT --> STATUS
STATUS -. updates .-> MS

%% ===================== STYLES =====================
classDef data fill:#eef9ff,stroke:#0077b6,color:#003049
class MS,LINE,TOK,AST,XAST,PLAN,STATUS data