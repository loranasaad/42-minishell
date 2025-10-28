fix history +
skip on whitespace only

1) /something bash shows: /something no such file or directory
					or:	Is a directory
in minishell it does nothing

2)	(solved) (it was not history problem but caused by readline, the first press after ctrl+c was not responsive) when ctrl c after writing something ( no enter) history is an empty space
it shouldn't be recorded in history. 

3) (solved)	Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
we get : waitpid: Interrupted system call
what we should get : ^C new prompt

4)	Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.

we get: new prompt
we should get: ^\ Quit (core dumped)

5)	export "x" not a valid identifier x should show what is not valid but we have empty " " 

6) Set the $PATH to a multiple directory value (directory1:directory2) and ensure that directories are checked in	order from left to right.

to do later

7) (fixed) ctrl c should should always show ^C 

8) (fixed) on heredoc ctrl c should quit
