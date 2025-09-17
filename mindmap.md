1) High-level layers

	┌───────────────────────────────────────────────────────────────────────────┐
	│                               APPLICATION                                 │
	│  REPL (Prompt & History)  |  Builtins Dispatcher  |  Exit Status ($?)     │
	└───────────────▲───────────────────────┬────────────────────────────────────┘
					│                       │
					│ command line          │ builtin calls
					│                       │
	┌───────────────┴───────────────────────▼────────────────────────────────────┐
	│                               CORE LOGIC                                   │
	│  Lexer  →  Parser (AST)  →  Expander ($VAR/$?)  →  Heredoc Prep            │
	│    tokens     AST (ND_PIPE/ND_CMD)       argv/redirs        temp fds       │
	└───────────────▲───────────────────────────────────────────────┬────────────┘
					│                                               │
					│ exec plan (expanded AST)                      │
	┌───────────────┴───────────────────────────────────────────────▼────────────┐
	│                               OS ADAPTERS                                   │
	│  Executor (fork/execve/dup2/pipe)  |  Path Resolution  |  Env List <-> envp │
	│  Signals (parent/child policies)    |  File/FD handling                     │
	└─────────────────────────────────────────────────────────────────────────────┘

2) Module Map

include/
  minishell.h         # core types (t_ms, t_env) + shared prototypes
  lexer.h parser.h expander.h heredoc.h exec.h builtins.h signals.h

src/
  main.c              # init t_ms, call repl(), cleanup
  repl.c              # readline loop; Ctrl-D exit; add_history

  lexer/              # tokenize: WORD, |, <, >, >>, << ; quote awareness
  parser/             # tokens -> AST (ND_PIPE, ND_CMD with argv + redirs)
  expander/           # quote removal; $VAR/$?; field splitting (unquoted)
  heredoc/            # << handling; expansion rules; temp fds; Ctrl-C cancel

  exec/               # pipeline setup; fork/dup2; redirs; wait & $? from last
    path.c            # PATH search; permission/isdirectory errors
    env_to_envp.c     # build char** for execve from list

  builtins/           # echo/pwd/env | cd/export/unset/exit
    dispatch.c        # is_builtin()/run_builtin(); parent-vs-child policy

  env/                # linked list ops: get/set/unset; init from envp

  signals.c           # one global int; handlers; parent/child modes

  utils/              # string/vec helpers; error printing; safe malloc/free



3) Data flow (command lifecycle)

  User line
  ▼
[REPL] readline() ──(string)──────────────────────▶
  ▼
[LEXER] string → tokens ──────────────────────────▶
  ▼
[PARSER] tokens → AST (ND_PIPE / ND_CMD) ─────────▶
  ▼
[EXPANDER] argv + redir targets expanded ─────────▶
  ▼
[HEREDOC] prepare temp fds for << ────────────────▶
  ▼
[EXECUTOR] build pipes → fork children → dup2 fds → execve/builtin
  ▼
[WAIT] parent waits; last child status → t_ms.last_status ($?)

4) Process/FD view for a pipeline (example: cat <in | grep x >>out)

PARENT:
  pipe(P0)
  ├─ fork -> C0 (cat)
  └─ fork -> C1 (grep)

CHILD C0:
  dup2(in_fd, STDIN)           # '< in'
  dup2(P0[1], STDOUT)          # write end of pipe
  close(unused fds)
  execve("cat", ...)

CHILD C1:
  dup2(P0[0], STDIN)           # read end of pipe
  dup2(out_fd_append, STDOUT)  # '>> out'
  close(unused fds)
  execve("grep", ...)

PARENT:
  close(P0[*]); wait for children; set $? from C1.

5) Signals & modes (parent vs child)
  At prompt (parent):
  SIGINT (Ctrl-C)  → clear line, print newline, redisplay prompt; set $?=130
  SIGQUIT (Ctrl-\) → ignored
  Ctrl-D (EOF)     → print "exit" and terminate shell

	In children (after fork, before exec/builtin):
	restore default handlers so external programs receive SIGINT/SIGQUIT
	if child killed by signal s → exit status = 128 + s

6) Key structures snapshot
	t_token  { kind, value, *next }
	t_redir  { kind(IN/OUT/APP/HDOC), target, *next }
	t_cmd    { char **argv, t_redir *redirs }
	t_ast    { type(ND_CMD/ND_PIPE), *left, *right, t_cmd *cmd }
	t_env    { key, value, *next }
	t_ms     { t_env *env, int last_status, int interactive }


──────────────────────────────────────────────
EXIT CODE LEGEND (Minishell vs Bash)
──────────────────────────────────────────────
0    → success (command executed without error)

1    → general error (command found but failed,
        e.g. `ls not_a_file`)

2    → syntax error in minishell (e.g. `| ls`, `> file`)

126  → command found but cannot be executed
        (permission denied, is a directory)

127  → command not found (PATH lookup failed)

128+n → terminated by signal n
        e.g. SIGINT (2)   → 130
             SIGQUIT (3)  → 131

130  → Ctrl-C killed foreground program
131  → Ctrl-\ killed foreground program

$?   → always holds the exit status of the last
        foreground pipeline (last command in it).
──────────────────────────────────────────────

				Architecture

minishell
├── main.c
│   └── Shell init (env copy, interactive check)
│   └── Calls repl()
│
├── repl.c
│   └── readline() input
│   └── add_history()
│   └── Handles Ctrl-D exit
│
├── signals.c
│   └── Signal Handlers
│       ├── SIGINT (Ctrl-C) → newline + prompt
│       ├── SIGQUIT (Ctrl-\) → ignore
│       └── EOF (Ctrl-D) → exit
│   └── One global int for signal number
│
├── lexer.c
│   └── Tokenize input
│       ├── WORD
│       ├── PIPE
│       ├── REDIRECT (<, >, >>, <<)
│       ├── QUOTES
│       └── VARIABLES
│   └── Produces t_token list
│
├── parser.c
│   └── Build AST
│       ├── ND_CMD
│       │    └── argv (char **)
│       │    └── redirs (t_redir list)
│       └── ND_PIPE
│   └── Detect syntax errors
│
├── expander.c
│   └── Variable Expansion
│   └── Quote Removal
│   └── Field Splitting
│   └── Exit status ($?)
│
├── heredoc.c
│   └── << handling
│   └── Expansion if unquoted delimiter
│   └── Ctrl-C cancels heredoc (status 130)
│
├── executor.c
│   └── External commands
│       ├── PATH resolution (path.c)
│       ├── execve()
│   └── Pipelines
│       ├── pipe creation
│       ├── fork processes
│       ├── dup2 redirection
│   └── Redirections
│       ├── open/close
│       └── dup/dup2
│   └── Wait & collect exit status
│
├── builtins.c (dispatcher)
│   ├── echo.c
│   ├── cd.c
│   ├── pwd.c
│   ├── export.c
│   ├── unset.c
│   ├── env.c
│   └── exit.c
│
├── env_utils.c
│   └── getenv / setenv / unsetenv
│   └── env_to_envp()
│   └── SHLVL, OLDPWD, etc.
│
├── utils.c
│   └── String helpers
│   └── Memory management
│   └── Error handling
│
└── include/
    ├── minishell.h (core structs & prototypes)
    ├── lexer.h / parser.h / exec.h / builtins.h / env.h
    └── signals.h / heredoc.h