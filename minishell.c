/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlongo <mlongo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:35:17 by fcarlucc          #+#    #+#             */
/*   Updated: 2023/09/05 17:49:25 by mlongo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_exit_status_cmd = 0;

void printTree(t_tree *node, int level, char *message)
{
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
	printf("%s\n", message);
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    if (node->type == SIMPLE_CMD) {
        t_simple_cmd *cmd = (t_simple_cmd *)node->content;
        printf("SIMPLE_CMD\n");
		print_tokens(cmd->redir_list);
		for (int i = 0; i < level; i++) {
  	    	printf("  ");
   		}
        print_tokens(cmd->cmd->cmd_arg);
		for (int i = 0; i < level; i++) {
  	    	printf("  ");
   		}
        print_tokens(cmd->cmd->cmd_name);
        print_tokens(cmd->env);
    } else if (node->type == OP) {
        printf("OP\n");
        char *cmd = (char *)node->content;
		printf("content : %s\n", cmd);
    } else if (node->type == PARENTHESI) {
        printf("PARENTHESIS\n");
        t_parenthesis *cmd = (t_parenthesis *)node->content;
		print_tokens(cmd->redir_list);
		printTree(cmd->tree, level, "ROOT PARENTHESIS");
    }

    printTree(node->left, level + 1, "LEFT");
    printTree(node->right, level + 1, "RIGHT");
}

char	**env_container(int action, void *arg)
{
	static char **env;

	if (action == 0)
		env = (char **)arg;
	return (env);
}

void	free_tokens(t_token *token_lst)
{
	t_token			*tmp;
	t_declaration	*type_decl;
	t_declaration	*tmpdecl;
	char			**args;
	int				i;

	i = 0;
	while (token_lst)
	{
		if (token_lst->token == OPERATOR || token_lst->token == OUT_FILE_APPEND || token_lst->token == OUT_FILE_TRUNC
				|| token_lst->token == IN_FILE_TRUNC || token_lst->token == OPERATOR || token_lst->token == CMD_NAME)
			free(token_lst->value);
		else if (token_lst->token == ENV_VAR_DECL || token_lst->token == ENV_VAR_UNSET)
		{
			type_decl = (t_declaration *)token_lst->value;
			while (type_decl)
			{
				tmpdecl = type_decl;
				type_decl = type_decl->next;
				free(tmpdecl->name);
				if (tmpdecl->value)
					free(tmpdecl->value);
				free(tmpdecl);
			}
		}
		else if (token_lst->token == CMD_ARG)
		{
			i = 0;
			args = (char **)token_lst->value;
			while(args[i])
			{
				free(args[i]);
				i++;
			}
			free(args);
		}
		tmp = token_lst;
		token_lst = token_lst->next;
		free(tmp);
	}
	free(token_lst);
}

void	free_tree(t_tree *tree)
{
	t_parenthesis	*par;
	t_simple_cmd	*simple_cmd;

	if (!tree)
		return ;

	if (tree->type == PARENTHESI)
	{
		par = (t_parenthesis *)tree->content;
		free_tokens((t_token *)par->redir_list);
		free_tree((t_tree *)par->tree);
		free(par);
	}
	else if (tree->type == SIMPLE_CMD)
	{
		simple_cmd = (t_simple_cmd *)tree->content;
		free_tokens(simple_cmd->redir_list);
		free_tokens(simple_cmd->env);
		if (simple_cmd->cmd)
		{
			free(simple_cmd->cmd->cmd_arg);
			free_tokens(simple_cmd->cmd->cmd_name);
			free(simple_cmd->cmd);
		}
		free(simple_cmd);
	}
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}

void	ft_free_all(t_token *token_lst, t_tree *tree)
{
	(void)tree;
	free_tokens(token_lst);
	free_tree(tree);
}

static char *path_create(char *s1, char *s2, char *s3, char *s4, char *s5)
{
	int i;
	int j = 0;
	char *str;

	str = malloc(strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + strlen(s5) + 1);
	i = 0;
	while (s1[j])
		str[i++] = s1[j++];
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	j = 0;
	while (s3[j])
		str[i++] = s3[j++];
	j = 0;
	while (s4[j])
		str[i++] = s4[j++];
	j = 0;
	while (s5[j])
		str[i++] = s5[j++];
	str[i] = 0;
	return (str);
}

static char *create(char *s1)
{
	int i;
	char *str;

	str = malloc(strlen(s1) + 1);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

static char *mini(void)
{
	char pwd[4096];
	char *path;
	char *name;
	char **dir;
	char **tmp;

	getcwd(pwd, sizeof(pwd));
	dir = ft_split(pwd, '/');
	tmp = dir;
	name = create(dir[2]);
	while (*dir)
		dir++;
	path = path_create("\033[31;49;3;1m", name, "@Mini$hell🔥:\033[32m~/", *--dir, "$ \033[0m");
	free_matrix(tmp);
	free(name);
	return (path);
}

void	signal_handler(int signum)
{
	(void)signum;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	signal(SIGQUIT, SIG_IGN);
	(void)argc;
	(void)argv;
	char	*str;
	char	**splitcmd;
	char	*fixed;
	char	*path;
	t_token	*token_list;
	t_tree	*tree;

	env_container(0, envp);

	token_list = NULL;
	while (1)
	{
		while (1)
		{
			unlink_here_docs(handle_list_heredocs(LIST));
			handle_list_heredocs(START);
			signal(SIGINT, signal_handler);
			path = mini();
			str = readline(path);
			free(path);
			if (str == NULL)
			{
				printf("\n");
				free(str);
				exit(0);
			}
			add_history(str);
			fixed = fix_syntax(str);
			if (!check(fixed))
			{
				free(fixed);
				free(str);
				if (last_exit_status_cmd == 130)
					break ;
				printf("Syntax error\n");
				last_exit_status_cmd = 2;
				break;
			}
			splitcmd = ft_split(fixed, ' ');
			free(fixed);
			token_list = tokenizer(splitcmd);
			// print_tokens(token_list);
			if (token_list)
				while (token_list->next)
					token_list = token_list->next;
			tree = tree_create(token_list, OP);
			// printTree(tree, 0, "ROOT");
			execute(tree, STDIN_FILENO, STDOUT_FILENO);
			if (token_list)
				while (token_list->prev)
					token_list = token_list->prev;
			// printf("%d\n", last_exit_status_cmd);
			free_matrix(splitcmd);
			ft_free_all(token_list, tree);
			free(str);
		}
	}
}
