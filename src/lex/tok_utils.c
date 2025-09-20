/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loasaad <loasaad@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:12:15 by loasaad           #+#    #+#             */
/*   Updated: 2025/09/20 20:44:17 by loasaad          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tok_new(t_tok kind, char *val, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->kind = kind;
	token->val = val;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}

void	tok_push_back(t_token **head, t_token *node)
{
	t_token	*w;

	if (!head || !node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	w = *head;
	while (w->next)
		w = w->next;
	w->next = node;
	return ;
}

void	free_tokens(t_token *t)
{
	t_token	*w;
	t_token	*destroy;

	w = t;
	while (w)
	{
		destroy = w;
		w = w->next;
		if (destroy->val)
			free(destroy->val);
		free(destroy);
	}
	return ;
}