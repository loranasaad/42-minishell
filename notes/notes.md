fix history +
skip on whitespace only

1) /something bash shows: /something no such file or directory
					or:	Is a directory
in minishell it does nothing

2)	when ctrl c after writing something ( no enter) history is an empty space
it shouldn't be recorded in history

3)	Try ctrl-C after running a blocking command like cat without arguments or grep “something“.
we get : waitpid: Interrupted system call
what we should get : ^C new prompt

4)	Try ctrl-\ after running a blocking command like cat without arguments or grep “something“.

we get: new prompt
we should get: ^\ Quit (core dumped)

5)	export "x" not a valid identifier x should show what is not valid but we have empty " " 

6) Set the $PATH to a multiple directory value (directory1:directory2) and ensure that directories are checked in	order from left to right.

to do later

7) using valgrind we get this:
==34704== Syscall param rt_sigaction(act->sa_flags) points to uninitialised byte(s)
==34704==    at 0x49715F3: __libc_sigaction (libc_sigaction.c:58)
==34704==    by 0x4015BB: init_prompt_signals (in /home/latabagl/minishell/minishell)
==34704==    by 0x401380: repl (in /home/latabagl/minishell/minishell)
==34704==    by 0x401350: main (in /home/latabagl/minishell/minishell)
==34704==  Address 0x1ffeffff78 is on thread 1's stack
==34704==  in frame #0, created by __libc_sigaction (libc_sigaction.c:43)

8) memory leaks

9) norm

10) file name change

11) tidy up h files

12) tester last time


