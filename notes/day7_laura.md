Day 7 – Laura: Expansion v1 (no field splitting)

Scope (what changes)
	•	Work inside your cmdspec builder (the function that turns [start..end) tokens into t_cmdspec).
	•	For every WORD the builder consumes (argv element or redir target):
	1.	Strip quotes according to context
	2.	Expand $VAR and $? (but never expand inside single quotes)
	3.	Keep the entire expanded text as one string (no field splitting yet)

You already have token->quoted (0 = none, 1 = single, 2 = double). Use it if reliable; otherwise infer while stripping quotes.

⸻

File layout & prototypes

Files
	•	src/parser/cmdspec_expand.c (new): helpers for quote removal + expansion
	•	src/parser/cmdspec.c (existing): call the helpers before pushing argv / redirs
	•	Headers: add prototypes in include/parser.h (or a small include/expand.h if you prefer)

// helpers (Laura)
int     is_name_char(int c);
char   *expand_dollar(const char *s, int *i, t_ms *ms, t_env *env);
char   *remove_quotes_and_expand(const char *in, int quote_mode, t_ms *ms, t_env *env);

Step 1 — Small helpers (pseudocode)

1) is_name_char

Purpose: recognize ENV var name chars: [A-Za-z0-9_]

int is_name_char(int c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return 1;
    if ((c >= '0' && c <= '9') || c == '_')
        return 1;
    return 0;
}
First char rule: you’ll enforce when scanning a name (allow _ or alpha to start).

⸻

2) expand_dollar

Purpose: called when you see $ in unquoted or double-quoted context.
Input: s (the whole string), *i (current index at $), ms, env.
Output: new malloc’ed string for the expansion, advance *i to the char after the variable name (or ?). Return "" for missing vars.

Pseudocode:

expand_dollar(s, &i, ms, env):
    // pre: s[i] == '$'
    i++                            // skip '$'
    if s[i] == '?' :
        i++                        // consume '?'
        return itoa(ms->last_status)
    // parse variable name
    if s[i] is alpha or '_' :
        start = i
        i++                        // consumed first char
        while s[i] is_name_char: i++
        name = substr(s, start, i - start)
        val  = env_get(env, name)  // returns const char* or NULL
        if val == NULL: return strdup("")      // missing -> empty
        return strdup(val)
    else:
        // not a valid var name -> POSIX shells keep '$' + next char behavior nuanced.
        // For our v1: treat as "$" alone -> literally "$"
        return strdup("$")

Note: In strict bash, $ followed by non-name char can behave differently (e.g., $-, $@). For v1 keep it simple as above.


⸻

3) remove_quotes_and_expand

Purpose: produce one malloc’ed string from a token value by:
	•	removing quotes
	•	expanding $ only in double-quoted or unquoted mode
	•	never expanding inside single quotes

Inputs:
	•	in: original token string
	•	quote_mode: 0 none, 1 single, 2 double (if reliable); if not, you can still scan and strip quotes
	•	ms, env

Pseudocode (one pass, push to dynamic buffer):

remove_quotes_and_expand(in, quote_mode, ms, env):
    init out = empty buffer
    i = 0
    if quote_mode == 1:  // single-quoted token
        // copy everything except the outer single quotes
        // and DO NOT expand
        while in[i]:
            if in[i] != '\'': buffer_push(out, in[i])
            i++
        return buffer_take(out)

    if quote_mode == 2:  // double-quoted token
        while in[i]:
            if in[i] == '"':
                i++      // drop quotes
            else if in[i] == '$':
                piece = expand_dollar(in, &i, ms, env)
                if (!piece) goto oom
                buffer_push_str(out, piece)
                free(piece)
            else:
                buffer_push(out, in[i]); i++
        return buffer_take(out)

    // quote_mode == 0 (unquoted token)
    while in[i]:
        if in[i] == '\'':
            // copy literal chars until next single quote, no expansion
            i++ // drop opening '
            while in[i] && in[i] != '\'':
                buffer_push(out, in[i]); i++
            if in[i] == '\'': i++ // drop closing '
        else if in[i] == '"':
            // double-quoted section: expand $
            i++ // drop opening "
            while in[i] && in[i] != '"':
                if in[i] == '$':
                    piece = expand_dollar(in, &i, ms, env)
                    if (!piece) goto oom
                    buffer_push_str(out, piece)
                    free(piece)
                else:
                    buffer_push(out, in[i]); i++
            if in[i] == '"': i++ // drop closing "
        else if in[i] == '$':
            piece = expand_dollar(in, &i, ms, env)
            if (!piece) goto oom
            buffer_push_str(out, piece)
            free(piece)
        else:
            buffer_push(out, in[i]); i++

    return buffer_take(out)

oom:
    buffer_free(out)
    return NULL

Notes:
	•	This supports mixed quotes inside an unquoted token, e.g., a" b "$USER' c' → one resulting string.
	•	Single-quoted parts are copied literally; double-quoted and unquoted parts expand $.

⸻

Step 2 — Integrate in the builder

Where: inside build_cmdspec_from_segment(...), when you see:
	•	A WORD for argv:

out = remove_quotes_and_expand(tok->val, tok->quoted, ms, env)
if (!out) return 0    // handle OOM
push to argv as-is (no splitting)

	A redirection operator (< > >> <<) followed by its target WORD:

target = remove_quotes_and_expand(next_tok->val, next_tok->quoted, ms, env)
if (!target) return 0
make t_redir node with that target

Heredoc note: for << limiter: if the limiter token was quoted, you must not expand in the heredoc body later. For now, just store the raw expanded limiter string and (optionally) a flag heredoc_quoted derived from next_tok->quoted == 1. You’ll use that in Day 8 to decide whether to expand inside heredoc content.

Memory discipline:
	•	On any allocation failure, free partial argv + redirs and return 0 (your existing free_cmdspec should cover it if you set things carefully).

⸻

Step 3 — Edge cases to match (v1 rules)
	•	$UNSET → "" (empty). Keep it as a single argv entry (empty string allowed).
	•	'' or "" alone → empty string argv entry.
	•	Redir targets expand: > "$OUT" uses the expanded string as the filename.
	•	$? becomes decimal string of ms->last_status.
	•	Long expansions (like $PATH) must be fully copied (no truncation).
	•	No field splitting yet: FOO="a b" then echo $FOO → one arg a b (with space inside).

⸻

Step 4 — Minimal tests (run manually)
	1.	echo "$USER" vs echo '$USER'
	•	First prints your username, second prints $USER.
	2.	echo $?
	•	Prints last status.
	3.	cat < "$IN" > "$OUT"
	•	Uses expanded values as filenames.
	4.	echo $UNSETX
	•	Prints empty line (just newline).

⸻

Common pitfalls & tips
	•	Don’t expand in single quotes—ever.
	•	Do expand in double quotes and unquoted contexts.
	•	Keep one string per WORD (no splitting), even if expansion introduces spaces.
	•	Avoid ternaries; keep functions <25 lines (split if needed).
	•	Use your existing ft_* utilities (e.g., ft_strlen, ft_strdup, ft_memcpy, dynamic buffer helpers if you have them).
	•	For buffers, either:
	•	a small growable buffer struct (double capacity), or
	•	count once + allocate (harder with expansion), or
	•	stitch pieces via temporary strs + ft_strjoin (watch allocations & frees).

⸻

Hand-off guarantee for Loran

After Laura integrates this:
	•	build_cmdspec_from_segment returns argv[] and redir targets already quote-free and expanded per v1 rules, ready for exec_one_cmd and (soon) pipelines.
	•	Loran doesn’t do extra expansion in executor.

If you want, I can draft ultra-small buffer helper prototypes (push char / push string / take-or-free) that meet Norm.