1) cat < a | echo
2) minishell> env | sort | grep -v SHLVL
3) export GHOST=123 | env | grep GHOST


1)
vboxuser@Ubuntu:~/Projects/leaks$ valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell 
==7983== Memcheck, a memory error detector
==7983== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==7983== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==7983== Command: ./minishell
==7983== 
minishell> cat < a | echo

==8032== 
==8032== HEAP SUMMARY:
==8032==     in use at exit: 191,880 bytes in 228 blocks
==8032==   total heap usage: 612 allocs, 384 frees, 215,341 bytes allocated
==8032== 
==8032== 2 bytes in 1 blocks are still reachable in loss record 1 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10FF20: ft_strjoin (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10E263: join_res_seg (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10E213: expand_dollar_sign (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10E11D: handle_var_expansion (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AF41: build_tredir (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10ACEC: handle_redir (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AB50: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 4 bytes in 1 blocks are still reachable in loss record 2 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10FE71: ft_strdup (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10B194: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 8 bytes in 1 blocks are still reachable in loss record 5 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10D5BD: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 8 bytes in 1 blocks are still reachable in loss record 6 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10D66A: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 16 bytes in 1 blocks are still reachable in loss record 16 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10D3E2: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 16 bytes in 1 blocks are still reachable in loss record 17 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10B1C8: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 32 bytes in 1 blocks are still reachable in loss record 21 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10D41E: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== 32 bytes in 1 blocks are still reachable in loss record 22 of 73
==8032==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8032==    by 0x10AEF6: build_tredir (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10ACEC: handle_redir (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10AB50: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==8032==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==8032== 
==8032== LEAK SUMMARY:
==8032==    definitely lost: 0 bytes in 0 blocks
==8032==    indirectly lost: 0 bytes in 0 blocks
==8032==      possibly lost: 0 bytes in 0 blocks
==8032==    still reachable: 118 bytes in 8 blocks
==8032==         suppressed: 191,762 bytes in 220 blocks
==8032== 
==8032== For lists of detected and suppressed errors, rerun with: -s
==8032== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

2)
minishell> env | sort | grep -v SHLVL
==9429== 
==9429== HEAP SUMMARY:
==9429==     in use at exit: 191,941 bytes in 230 blocks
==9429==   total heap usage: 632 allocs, 402 frees, 215,713 bytes allocated
==9429== 
==9429== 12 bytes in 1 blocks are still reachable in loss record 7 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10D66A: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 16 bytes in 1 blocks are still reachable in loss record 12 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10B1C8: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 16 bytes in 1 blocks are still reachable in loss record 13 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10D5BD: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 19 bytes in 4 blocks are still reachable in loss record 14 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10FE71: ft_strdup (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10B194: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 24 bytes in 1 blocks are still reachable in loss record 17 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10D3E2: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 32 bytes in 1 blocks are still reachable in loss record 20 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10B24B: strv_push_helper (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10B219: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== 48 bytes in 1 blocks are still reachable in loss record 24 of 72
==9429==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9429==    by 0x10D41E: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9429==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9429== 
==9429== LEAK SUMMARY:
==9429==    definitely lost: 0 bytes in 0 blocks
==9429==    indirectly lost: 0 bytes in 0 blocks
==9429==      possibly lost: 0 bytes in 0 blocks
==9429==    still reachable: 167 bytes in 10 blocks
==9429==         suppressed: 191,774 bytes in 220 blocks
==9429== 
==9429== For lists of detected and suppressed errors, rerun with: -s
==9429== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

3)
minishell> export GHOST=123 | env | grep GHOST
==9755== 
==9754== 
==9754== HEAP SUMMARY:
==9754==     in use at exit: 192,001 bytes in 232 blocks
==9754==   total heap usage: 695 allocs, 463 frees, 216,776 bytes allocated
==9754== 
==9755== HEAP SUMMARY:
==9755==     in use at exit: 192,022 bytes in 233 blocks
==9755==   total heap usage: 692 allocs, 459 frees, 216,734 bytes allocated
==9755== 
==9754== 12 bytes in 1 blocks are still reachable in loss record 6 of 72
==9755== 12 bytes in 1 blocks are still reachable in loss record 6 of 71
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10D66A: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10D66A: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755== 
==9754== 
==9755== 16 bytes in 1 blocks are still reachable in loss record 11 of 71
==9754== 15 bytes in 3 blocks are still reachable in loss record 10 of 72
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10D5BD: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10FE71: ft_strdup (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10B194: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9755== 
==9754==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9755== 24 bytes in 1 blocks are still reachable in loss record 13 of 71
==9754== 16 bytes in 1 blocks are still reachable in loss record 12 of 72
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10D3E2: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10B1C8: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9755== 
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9755== 28 bytes in 4 blocks are still reachable in loss record 16 of 71
==9754== 16 bytes in 1 blocks are still reachable in loss record 13 of 72
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10FE71: ft_strdup (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10B194: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10D5BD: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755== 
==9754== 24 bytes in 1 blocks are still reachable in loss record 15 of 72
==9755== 48 bytes in 1 blocks are still reachable in loss record 20 of 71
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754==    by 0x10D3E2: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9754== 24 bytes in 1 blocks are still reachable in loss record 16 of 72
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754==    by 0x10B24B: strv_push_helper (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10B219: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10D41E: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9755== 
==9755== 48 bytes in 2 blocks are still reachable in loss record 22 of 71
==9755==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9754== 48 bytes in 1 blocks are still reachable in loss record 22 of 72
==9755==    by 0x10B24B: strv_push_helper (in /home/vboxuser/Projects/leaks/minishell)
==9754==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==9755==    by 0x10B219: strv_push (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10D41E: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10AC5D: handle_word (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10AB10: build_cmdspec_from_segment (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10D4DC: exec_pipeline (in /home/vboxuser/Projects/leaks/minishell)
==9754==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x10C398: exec_run_node (in /home/vboxuser/Projects/leaks/minishell)
==9754== 
==9755==    by 0x10975B: repl (in /home/vboxuser/Projects/leaks/minishell)
==9755==    by 0x1095C5: main (in /home/vboxuser/Projects/leaks/minishell)
==9755== 
==9754== LEAK SUMMARY:
==9755== LEAK SUMMARY:
==9755==    definitely lost: 0 bytes in 0 blocks
==9754==    definitely lost: 0 bytes in 0 blocks
==9755==    indirectly lost: 0 bytes in 0 blocks
==9754==    indirectly lost: 0 bytes in 0 blocks
==9755==      possibly lost: 0 bytes in 0 blocks
==9754==      possibly lost: 0 bytes in 0 blocks
==9755==    still reachable: 176 bytes in 10 blocks
==9754==    still reachable: 155 bytes in 9 blocks
==9755==         suppressed: 191,846 bytes in 223 blocks
==9754==         suppressed: 191,846 bytes in 223 blocks
