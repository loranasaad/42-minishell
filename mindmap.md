```mermaid
flowchart TD

%% ===================== ENTRY & STATE =====================
    M["main.c: main(argc, argv, envp)"]
    IE["env/init_env(envp) → t_env*"]
    SS["signals/setup_interactive_handlers()"]
    R["repl.c: repl(t_ms*)"]

    MS[/"t_ms (env, last_status, interactive)"/]:::data

    M --> IE
    M --> SS
    M --> R
    IE -. updates .-> MS
    SS -. config .-> MS

%% ===================== REPL LOOP =====================
    RL["readline(prompt)"]
    AH["add_history(line)"]
    EXITD["print \"exit\" and return"]

    LINE[/"line : string"/]:::data

    R --> RL
    RL -->|non-null| LINE
    RL -->|NULL (Ctrl-D)| EXITD
    LINE -->|"non-empty"| AH

%% ===================== FRONTEND → CORE =====================
    LEX["lexer/lex(line) → tokens"]
    TOK[/"tokens : t_token*"/]:::data
    PAR["parser/parse(tokens) → AST"]
    AST[/"AST : t_ast*"/]:::data
    EXP["expander/expand(AST, t_ms*)"]
    XAST[/"expanded AST"/]:::data
    HD["heredoc/prepare(AST, t_ms*)"]
    PLAN[/"ready to exec (fds prepped)"/]:::data

    LINE --> LEX --> TOK --> PAR --> AST --> EXP --> XAST --> HD --> PLAN

%% ===================== EXECUTION =====================
    EXE["exec/exec_ast(AST, t_ms*)"]
    PLAN --> EXE

    F["fork N children / build pipes"]
    EXE --> F

    %% Child setup path
    CSET["child: dup2 pipes + redirs"]
    F --> CSET

    DEC{"builtin?"}
    CSET --> DEC

    BPAR["builtins/run_in_parent(argv, t_ms*)"]
    BCHILD["builtins/run_in_child(argv, t_ms*)"]
    PATHR["exec/path_resolve(argv[0], $PATH)"]
    ENV2["env/env_to_envp(t_env*) → char**"]
    EXECVE["execve(binary, argv, envp)"]

    %% Builtin decision
    DEC -->|single cmd (no pipe)| BPAR
    DEC -->|in pipeline| BCHILD
    DEC -->|no (external)| PATHR

    %% External flow
    PATHR --> ENV2 --> EXECVE

    %% Parent wait & status
    PWAIT["parent: close fds + wait last"]
    STATUS[/"$? : int (last_status)"/]:::data
    EXE --> PWAIT --> STATUS
    STATUS -. updates .-> MS

%% ===================== STYLES =====================
classDef data fill:#eef9ff,stroke:#0077b6,color:#003049