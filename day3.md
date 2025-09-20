2) Plan of attack

i = 0
tokens = []
while line[i] != '\0':
    skip_spaces()

    if end: break

    if next is '|' : push(TK_PIPE); i++
    else if next is '<':
        if line[i+1]=='<': push(TK_HDOC); i+=2
        else               push(TK_IN);   i+=1
    else if next is '>':
        if line[i+1]=='>': push(TK_APP);  i+=2
        else               push(TK_OUT);  i+=1
    else:
        // read WORD
        start = i
        buf = ""          // grow string
        word_quoted = 0   // 0 none, 1 single, 2 double (store last opened if whole word is a single quoted chunk; if mixed, you can set 0 and let expander track per-char if you want)
        while not end and not is_space(line[i]) and not is_meta_at(i):
            if line[i] == '\'':
                i++; if no closing '\'' → error
                append chars until next '\'' to buf
                i++; mark that part as single-quoted (for simple impl, if the entire word was a single-quoted chunk, set quoted=1)
            else if line[i] == '"':
                i++; if no closing '"' → error
                append chars until next '"' to buf
                i++; mark that part as double-quoted (if entire word was one dq chunk, set quoted=2; for mixed, you can leave quoted=0 and let expander handle internally)
            else:
                append line[i] to buf; i++
        push WORD(buf, quoted)


3) Error handling

Detect and report:
	•	Unclosed ' or " → set *lex_status = 2, free any built tokens, return NULL.
	•	Lexer should not diagnose | position or “missing filename after redir” — that’s typically the parser’s job. If you want to pre-check obvious ||, you can, but keep it in parser for cleanliness.

⸻

4) Memory discipline
	•	Every token’s val is a fresh malloced string (for WORD and possibly for redir targets if you decide to store them as tokens like < followed by a WORD).
	•	On any error: free all tokens built so far (free_tokens).
	•	free_tokens walks the list, frees val if non-NULL, then the node.

⸻

5) Minimal helpers you’ll likely write
	•	static int is_space(int c);
	•	static int is_meta(char c); → returns true for |<>
	•	static int is_meta_at(const char *s, int i); → to detect <</>>
	•	static char *substr_dup(const char *s, int start, int end); or a small grow-buffer routine.
	•	static t_token *tok_new(t_tok kind, const char *val, int quoted);
	•	static void tok_push_back(t_token **head, t_token *node);

6) Unit-test harness (quick & dirty)

Make a temporary lexer_test.c (not part of final build) that:
	•	Reads example lines from argv or stdin,
	•	Calls lex_line(line, &st),
	•	Prints the chain like:
WORD("cat") | IN("<") | WORD("in.txt") | PIPE("|") | WORD("grep") | WORD("foo $USER"[dq]) | APP(">>") | WORD("out.log")
	•	Frees tokens.

Test vectors
	•	echo hi
	•	cat < in | grep "x y" >> out
	•	<<EOF cat
	•	echo "unterminated → should fail with status 2
	•	echo 'a b'c"d e"f → single mixed word a bc d ef (your lexer yields WORD with raw combined text; expander later handles quote removal)

⸻

7) Integration decisions (so you don’t paint yourself in a corner)
	•	Do not expand here. Expansion is Stage 4.
	•	Do not open files here. Redirs are applied in the executor.
	•	Do not validate grammar here. Keep that in the parser (Stage 4/5).
	•	Keep tokens simple: for redirections, you’ll usually have sequence < then WORD(filename). Parser will bind them into a redir list attached to a command node.

⸻

8) Today’s To-Do (Loran)
	1.	Add the token enum/struct and prototypes to minishell.h.
	2.	Create src/lexer/lex.c and implement lex_line() with the scanner loop above.
	3.	Add src/lexer/ to your Makefile sources.
	4.	Implement free_tokens().
	5.	Build a tiny printer (debug only) to validate output.
	6.	Run the test vectors and ensure:
	•	Tokens and kinds match
	•	Unclosed quotes → error path frees everything, returns NULL, sets *lex_status=2.



	perfect — here’s a tight, code-free implementation plan for the lexer based on that pseudocode. Follow it in order and you’ll end up with a clean lex_line().

Plan of attack (no full code)

A) Setup & API shape
	1.	In minishell.h, declare:
	•	t_tok enum (TK_WORD, TK_PIPE, TK_IN, TK_OUT, TK_APP, TK_HDOC).
	•	t_token { t_tok kind; char *val; int quoted; struct s_token *next; }.
	•	t_token *lex_line(const char *line, int *lex_status);
	•	void free_tokens(t_token *t);
	2.	Behavior contract for lex_line():
	•	On success: returns head of token list, sets *lex_status = 0.
	•	On error (e.g., unclosed quotes): frees anything built, returns NULL, sets *lex_status = 2 (we’ll map this to $?=2 later).

⸻

B) Tiny helpers (static, in lex.c)

Write these first; they keep the scanner readable:
	•	is_space(int c) → spaces/tabs only (keep it simple).
	•	is_meta(char c) → returns true for |, <, >.
	•	peek2(const char *s, int i, char a, char b) → true if s[i]==a && s[i+1]==b.
	•	tok_new(kind, val, quoted) → alloc node (val can be NULL for operators).
	•	tok_push_back(&head, node) → append to singly linked list.
	•	slice_dup(const char *s, int start, int end) → malloc a substring (end exclusive).
(Or use a tiny grow-buffer struct if you prefer append-by-append.)
	•	unclosed_error_cleanup(&head, lex_status) → free list, set *lex_status=2, return NULL.

Keep each helper tiny and focused.

⸻

C) Scanner skeleton (single pass)

Maintain an index i walking the line:
	1.	Skip spaces
While line[i] is space/tab, i++. If end → done.
	2.	Operators first (single token lookahead)
	•	If line[i] == '|' → push TK_PIPE, i += 1.
	•	Else if <:
	•	If peek2(line,i,'<','<') → push TK_HDOC, i += 2.
	•	Else → push TK_IN, i += 1.
	•	Else if >:
	•	If peek2(line,i,'>','>') → push TK_APP, i += 2.
	•	Else → push TK_OUT, i += 1.
After pushing an operator, continue to loop top (skip spaces again).
	3.	WORD collection (otherwise)
	•	Initialize an empty buffer (or mark start = i if you prefer slice accumulation).
	•	Track a quoted flag for the overall word:
	•	Start with 0 (unquoted).
	•	If the entire word is a single quoted segment '... → set quoted=1.
	•	If the entire word is a double quoted segment "..." → set quoted=2.
	•	If the word mixes quoted/unquoted segments, you can keep quoted=0 (the expander will still remove quotes by scanning val later).
	•	Loop until you hit end, space, or a meta char at top level:
	•	If single quote:
	•	Consume opening '.
	•	Find the next '. If none → error (unclosed); do cleanup + return NULL.
	•	Append the raw contents between the quotes to the buffer (excluding the quotes).
	•	Optionally set quoted if the whole word is just this one quoted chunk.
	•	Advance i past closing '.
	•	Else if double quote:
	•	Same as above but for "; do not expand here — just copy contents.
	•	Unclosed " → error cleanup.
	•	Else if normal char (not space, not meta):
	•	Append to buffer, i++.
	•	Else (space/meta) → stop the word.
	•	When the loop ends, push TK_WORD with val = buffer and quoted as computed.
	4.	Repeat from step 1 until line[i] == '\0'.

⸻

D) Error handling policy
	•	Only error you catch in the lexer: unclosed quotes.
Action: free everything you built, set *lex_status=2, return NULL.
	•	Don’t try to validate grammar here (like ||, | at ends, or missing filename after >). That’s the parser’s job.

⸻

E) Memory discipline
	•	Every val allocated must be freed by free_tokens() later.
	•	On any early return due to error, free the list you’ve built so far.

⸻

F) Quick manual tests (print tokens)

Make a temporary printer (debug-only) to print:
[WORD:"cat" q=0] [IN:"<"] [WORD:"in.txt"] [PIPE:"|"] [WORD:"grep"] [WORD:"foo $USER" q=2] [APP:">>"] [WORD:"out.log"]

Test inputs:
	•	echo hi
	•	cat < in | grep "x y" >> out
	•	<<EOF cat
	•	echo "unterminated → should return NULL, status 2
	•	echo 'a b'c"d e"f → one WORD "a bc d ef" (combined raw text; expander will remove quotes later)
	•	   ls → leading spaces ignored
	•	ls|wc → operators with no spaces still recognized

⸻

G) Integration notes (so you’re ready for Stage 4)
	•	The parser will later read this linear list and build the AST:
	•	WORD ... sequence until | → one command node with argv and redirs.
	•	<, >, >>, << will be associated with the nearest command to the left.
	•	Don’t attempt expansion now. The expander will:
	•	remove quotes,
	•	expand $VAR and $? (not inside single quotes),
	•	split fields for unquoted expansions.

⸻

H) Minimal milestones (so you can checkpoint)
	1.	Operators tokenize correctly (including <</>>).
	2.	Single-quoted words collect literally; unclosed quote → error 2.
	3.	Double-quoted words collect literally; unclosed quote → error 2.
	4.	Mixed segments form one WORD value.
	5.	No memory leaks on success or on error (run valgrind on your token tests).

⸻

When you finish steps A–D, run the tests in F and paste a short dump of the tokens for:
cat  < in.txt |  grep "foo $USER"  >> out.log







WORD READER

Pass A — COUNT (state machine)

Inputs: line, start_i (already confirmed: not space, not |<>)

Scratch:
	•	i = start_i
	•	len = 0 (number of chars that will end up in the word, without quotes)
	•	segments = 0 (how many segments we will copy)
	•	saw_sq = 0, saw_dq = 0 (to compute the final quoted flag)

Loop while:
	•	line[i] exists, and
	•	not is_space(line[i]), and
	•	not is_meta(line[i]):

Cases:
	1.	Single quote ' at i
	•	i++ (skip opening quote)
	•	Find next '. If none → error (return -1).
	•	Let j point to the closing '.
	•	Increase len by j - i (the inside length).
	•	saw_sq = 1, segments++.
	•	Set i = j + 1 (past closing quote).
	2.	Double quote " at i
	•	i++ (skip opening)
	•	Find next ". If none → error.
	•	Let j be closing "; add j - i to len.
	•	saw_dq = 1, segments++.
	•	i = j + 1.
	3.	Normal char (not space/meta/quote)
	•	Optionally, scan a run of normal chars until the next space/meta/quote, and add that run length to len.
	•	segments++ for this run.
	•	Advance i to the first stopping char (don’t consume it).

Stop when you hit space/meta/end. Set *end_i = i.
Return len (≥0). If any unclosed quote, return -1.

Compute out_quoted (only if needed in Pass A; or compute in read_word after copy):
	•	If segments == 1 and saw_sq == 1 and saw_dq == 0 → 1
	•	Else if segments == 1 and saw_dq == 1 and saw_sq == 0 → 2
	•	Else → 0

Rationale: If the entire word is exactly one quoted segment, mark it as quoted; otherwise it’s mixed/unquoted.

⸻

Allocation step
	•	If len < 0 → failure (unclosed quotes).
	•	Allocate dst = malloc(len + 1). If NULL → handle as you usually do.
	•	Now do Pass B to fill dst.

⸻

Pass B — COPY (mirror of pass A)

Inputs: line, start_i, end_i, dst (size len + 1)

Scratch:
	•	i = start_i
	•	k = 0 (write index in dst)

Loop while i < end_i:
	•	If ' → i++ (skip), copy until next ' into dst[k..], then i++ (skip closing).
	•	Else if " → i++, copy until next " into dst[k..], i++.
	•	Else → copy a run of normal chars (until space/meta/quote/end_i), advancing both i and k.

Terminate with dst[k] = '\0'.

This pass should not error: you already validated quotes in Pass A.
You’re just mirroring the same segmenting logic and copying without the quotes.

⸻

Integrate into lex_line()

In the main scanner:
	•	Skip spaces. If end → break.
	•	If operator:
	•	<< → push TK_HDOC, i += 2
	•	>> → push TK_APP, i += 2
	•	<  → push TK_IN,   i += 1
	•	>  → push TK_OUT,  i += 1
	•	|  → push TK_PIPE, i += 1
	•	continue
	•	Else (WORD):
	•	Call read_word(line, i, &i_after, &str, &quoted)
	•	If fail:
	•	*lex_status = 2; free_tokens(head); return NULL;
	•	Else:
	•	tok_push_back(&head, tok_new(TK_WORD, str, quoted));
	•	i = i_after;
	•	Loop.

At end: *lex_status = 0; return head;

⸻

Quick sanity tests
	•	echo hi → WORD “echo”, WORD “hi” (quoted=0)
	•	cat<in|grep"x y">>out → WORD “cat”, IN, WORD “in”, PIPE, WORD “grep”, WORD “x y”(q=2), APP, WORD “out”
	•	<<EOF cat → HDOC, WORD “EOF”, WORD “cat”
	•	echo "unterminated → fail, lex_status=2
	•	'abc' → WORD “abc” (q=1)
	•	"abc" → WORD “abc” (q=2)
	•	a"b c"d → WORD “abcd” (q=0)

⸻

Notes / pitfalls to avoid
	•	Don’t consume the space/meta that ends the word; leave it for the outer loop to process next.
	•	Be careful when scanning runs: stop on space, |, <, >, ', " (quotes start a quoted segment).
	•	In Pass A, if you use ft_strchr to find the closing quote, compute indices carefully.
	•	Return only one error from lexer: unclosed quotes → status=2.

That’s it — implement this plan and your lexer’s word reading will be fast, clean, and compliant, without realloc.