

#include "minishell.h"
#include "parser.h"

static void		free_redirs(t_redir **redirs);
static void		redir_several_fields_error(const char *val);

void	free_cmdspec(t_cmdspec *spec)
{
	free_str_arr(&(spec->argv));
	free_redirs(&(spec->redirs));
	return ;
}

static void	free_redirs(t_redir **redirs)
{
	t_redir	*w;
	t_redir	*destroy;

	if (!redirs)
		return ;
	w = *redirs;
	while (w)
	{
		destroy = w;
		w = w->next;
		if (destroy->kind == R_HDOC && destroy->hdoc_fd >= 0)	//Loran: added safety closes heredoc fd if still open
			close(destroy->hdoc_fd);
		free(destroy->target);
		free(destroy);
	}
	*redirs = NULL;
}

// build a t_redir struct
t_redir	*build_tredir(t_token *tok, t_ms *ms)
{
	t_redir	*redir;
	int		status;
	int		multiple;

	if (!tok || !tok->next || !tok->next->val)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->kind = tok->kind - TK_IN;
	redir->target = handle_var_expansion(tok->next->val,
			tok->next->quoted, ms, &status);
	multiple = several_fields(tok->next->quoted, redir->target);
	if (!redir->target || !status || multiple)
	{
		if (multiple)
			redir_several_fields_error(tok->next->val);
		free(redir->target);
		free(redir);
		return (NULL);
	}
	redir->hdoc_exp = -1;
	redir->hdoc_fd = -1;
	redir->next = NULL;
	return (redir);
}

t_redir	*build_heredoc(t_token *tok)
{
	t_redir	*redir;

	if (!tok || !tok->next || !tok->next->val)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->kind = R_HDOC;
	redir->hdoc_fd = -1;
	redir->target = ft_strdup(tok->next->val);
	if (!redir->target)		//Loran: added for safety
	{
		if (multiple)
			redir_several_fields_error(tok->next->val);
		free(redir->target);
		free(redir);
		return (NULL);
	}
	if (tok->next->quoted == 0)
		redir->hdoc_exp = 1;
	else
		redir->hdoc_exp = 0;
	redir->next = NULL;
	return (redir);
}

static void	redir_several_fields_error(const char *val)
{
	const char	*s1;
	const char	*s2;

	s1 = "minishell: ";
	s2 = ": ambiguous redirect\n";
	write(2, s1, ft_strlen(s1));
	write(2, val, ft_strlen(val));
	write(2, s2, ft_strlen(s2));
}
