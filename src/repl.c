#include "minishell.h"

void	replt(t_ms *ms)
{
	char	*line;

	(void)ms;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break;
		free (line);
	}
}

