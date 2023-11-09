/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abuonomo <abuonomo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 11:51:13 by alessiolong       #+#    #+#             */
/*   Updated: 2023/11/09 18:55:47 by abuonomo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tree_node_operator(t_token *token_lst, t_tree **tree)
{
	(*tree)->type = OP;
	(*tree)->content = token_lst->value;
	token_lst = token_lst->prev;
	(*tree)->left = tree_create(token_lst, OP);
	token_lst = token_lst->next;
	token_lst = token_lst->next;
	(*tree)->right = tree_create(token_lst, SIMPLE_CMD);
}

t_tree	*simple_cmd_calling(t_token *token_lst, t_tree *tree)
{
	if (verify_parenthesis(token_lst))
	{
		tree->type = PARENTHESI;
		tree->content = parenthesis_node(token_lst);
		return (tree);
	}
	tree->type = SIMPLE_CMD;
	tree->content = simple_cmd_redirections((token_lst));
	simple_cmd(token_lst, tree->content);
	return (tree);
}

t_tree	*op_cmd_calling(t_token *token_lst, t_tree *tree)
{
	while (token_lst->prev != NULL)
	{
		if (token_lst->token == OPERATOR)
		{
			tree_node_operator(token_lst, &tree);
			return (tree);
		}
		else if (token_lst->token == PARENTHESIS)
			token_lst = skip_back_parenthesis(token_lst);
		else
			token_lst = token_lst->prev;
	}
	return (NULL);
}

t_token	*norminette_tree_create(t_token *token_lst)
{
	if (token_lst->token == PARENTHESIS)
		return (skip_back_parenthesis(token_lst));
	else
		return (token_lst->prev);
}

t_tree	*tree_create(t_token *token_lst, t_tree_enum calling)
{
	t_tree	*tree;

	if (!token_lst)
		return (NULL);
	tree = (t_tree *)malloc(sizeof(t_tree));
	tree_init_norm(tree);
	if (calling == SIMPLE_CMD)
		return (simple_cmd_calling(token_lst, tree));
	else if (calling == OP)
	{
		while (token_lst->prev != NULL)
		{
			if (token_lst->token == OPERATOR)
			{
				tree_node_operator(token_lst, &tree);
				return (tree);
			}
			else
				token_lst = norminette_tree_create(token_lst);
		}
	}
	free(tree);
	return (tree_create(token_lst, SIMPLE_CMD));
}
