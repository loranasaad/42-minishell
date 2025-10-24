
#include "minishell.h"

/*print is for exit : 1 if single command, 0 in pipeline
 Exit prints "exit" as single cmd and never print in the pipeline 
 so it need to be changed in the execution part
 In exec_one.c : in exec_stateful I have builtin_dispatch(spec->argv, ms, &rc, 1);
 and in exec_one_cmd builtin_dispatch(spec->argv, ms, &rc, 0); although it doesnt matter
 here because exit will never be called there if I understand that correctly
 In exec_pipeline.c : in exec_child builtin_dispatch(spec.argv, ms, &rc, 0);
*/
void	builtin_dispatch(char **argv, t_ms *ms, int *rc, int print)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		*rc = builtin_echo(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		*rc = builtin_pwd(argv);
	else if (ft_strcmp(argv[0], "env") == 0)
		*rc = builtin_env(argv, ms);
	else if (ft_strcmp(argv[0], "cd") == 0)
		*rc = builtin_cd(argv, ms);
	else if (ft_strcmp(argv[0], "export") == 0)
		*rc = builtin_export(argv, ms);
	else if (ft_strcmp(argv[0], "unset") == 0)
		*rc = builtin_unset(argv, ms);
	else if (ft_strcmp(argv[0], "exit") == 0)
		*rc = builtin_exit(argv, ms, print);
}
