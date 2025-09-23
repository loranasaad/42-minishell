Laura (B) — Env utilities + Command spec + PATH resolution



Goal: hand Loran a clean, ready-to-run “command spec” (argv[] + redirs) and env helpers (including envp build and PATH search).



Files & APIs (add these prototypes; implement internals yourself)



1) Env getters & envp builder

File: src/env/env_utils.c (header include/env.h or reuse minishell.h if you prefer)

const char *env_get(t_env *env, const char *key);          // returns pointer into list (do NOT free)

char      **env_to_envp(t_env *env);                       // malloc'd NULL-terminated array "KEY=VAL”

Notes:

env_get: iterate list; exact key match; return value or NULL.

env_to_envp: count pairs, allocate char **, join key, '=', value into each slot.



2) PATH resolution (env-aware)

File: src/env/path.c

char *find_in_path(const char *name, t_env *env);           // malloc’d full path or NULL

Behavior:

If name contains '/': test directly with access(name, X_OK); if OK → strdup(name), else NULL.

Else: env_get(env, "PATH"); split by :; try dir + "/" + name with access(X_OK); return first hit (malloc’d).

Return NULL if no hit or PATH missing/empty.



3) Command spec (from one AST_CMD segment)

File: src/parser/cmdspec.c (header include/parser.h adds the types)

Types to add to parser.h:

typedef enum e_rkind { R_IN, R_OUT, R_APP, R_HDOC } t_rkind;



typedef struct s_redir {

    t_rkind           kind;

    char             *target;  // filename or heredoc limiter (dup’d)

    struct s_redir   *next;

} t_redir;



typedef struct s_cmdspec {

    char    **argv;    // NULL-terminated

    t_redir *redirs;   // NULL if none

} t_cmdspec;

Builders:

int  build_cmdspec_from_segment(t_token *start, t_token *end, t_cmdspec *out); // 1 ok, 0 err

void free_cmdspec(t_cmdspec *spec);

Rules:

Walk [start..end):

TK_WORD → push val into a growable list → later argv (NULL-terminated).

TK_IN/TK_OUT/TK_APP/TK_HDOC → take next token (validator guarantees WORD), create a t_redir node with target = strdup(next->val); append to list; skip the filename token.

If argv[0] == NULL (pure redirections): keep it; Loran will decide how to handle (today: apply redirs to a no-op or treat as ok).



4) Small helpers (optional, re-usable)

File: src/util/strv.c

void  free_strv(char **v);                  // free vector of strings (NULL-terminated)

char **strv_push(char **v, const char *s);  // grow v, append strdup(s), return new v or NULL

Keeps your cmdspec builder clean and leak-free.

Acceptance for Laura

env_get(ms->env,"HOME") works.

env_to_envp(ms->env) returns a valid char ** of KEY=VAL pairs.

find_in_path("ls", env) returns a full path (e.g., /bin/ls) if present.

build_cmdspec_from_segment turns:

grep h > out.txt → argv=["grep","h",NULL], redirs [R_OUT "out.txt"]

cat < in >> out → argv=["cat",NULL], redirs [R_IN "in"] -> [R_APP "out"]