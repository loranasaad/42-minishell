
#include "minishell.h"
#include "parser.h"

static char	*join_res_seg(char *result, char *segment);

// return malloced str of input with var expansion; status is 0 if malloc failed or { not closed
char	*handle_var_expansion(char *str, int quoted, t_ms *ms, int *status)
{
	char	*result;
	char	*tilde_expanded;

	if (!str)
		return (str);
	if (!str[0])
		return (ft_strdup(""));
	tilde_expanded = expand_tilde(str, quoted, ms);
	if (!tilde_expanded)
	{
		*status = 0;
		return (NULL);
	}
	if (quoted == 0 || quoted == 2)
		result = expand_dollar_sign(tilde_expanded, ms);
	else if (quoted == 1)
		result = ft_strdup(tilde_expanded);
	free(tilde_expanded);
	if (!result)
	{
		*status = 0;
		return (NULL);
	}
	*status = 1;
	return (result);
}

char	*expand_dollar_sign(char *str, t_ms *ms)
{
	int		i;
	int		end;
	char	*result;
	char	*segment;

	i = 0;
	result = ft_strdup("");
	while (1)
	{
		if (!result)
			return (NULL);
		segment = get_next_segment(str, &i, ms, &end);
		if (!segment && end)
			break ;
		if (!segment && !end)
		{
			free(result);
			result = NULL;
			break ;
		}
		result = join_res_seg(result, segment);
	}
	return (result);
}

static char	*join_res_seg(char *result, char *segment)
{
	char	*tmp;

	tmp = result;
	result = ft_strjoin(result, segment);
	free(tmp);
	free(segment);
	return (result);
}
