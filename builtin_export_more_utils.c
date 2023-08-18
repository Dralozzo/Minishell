/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_more_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnicoter <lnicoter@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 20:30:03 by lnicoter          #+#    #+#             */
/*   Updated: 2023/08/18 15:00:40 by lnicoter         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void insert_declaration(t_declaration **list, int concatenation, const char *name, const char *value)
{
	t_declaration *new_declaration = malloc(sizeof(t_declaration));
	if (!new_declaration)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	new_declaration->concatenation = concatenation;
	new_declaration->name = strdup(name);
	if (value)
		new_declaration->value = strdup(value);
	else
		new_declaration->value = NULL;
	new_declaration->next = *list;

	*list = new_declaration;
}

//in realtà questo mi serve a simulare la cosa che fa manu
int	put_arguments_in_list(t_declaration *identity, char *command_line)
{
	int	i;

	i = 0;
	if (!ft_isalpha(command_line[0]))
	{
		printf("\033[1;31mbash: export: `%s': not a valid identifier\n", command_line);
		return (0);
	}
	while (command_line[i])
	{
		if (!ft_isalnum(command_line[i]) && command_line[i]!= '=' && command_line[i] != '+')
		{
			printf("\033[1;31mbash: export: `%s': not a valid identifier\n", command_line);
			return (0);
		}
		i++;
	}
	return (1);
}

void	print_list(t_declaration *list)
{
	while (list != NULL)
	{
		printf("name %s, conc %d, value %s\n", list->name, list->concatenation, list->value);
		list = list->next;
	}
}

void	arguments_separation(char **command_line, int conc, t_declaration **identity)
{
	char	**separated_args;
	int		plus_finder;
	int		i;

	plus_finder = 0;
	i = 0;
	while(command_line[++i])
	{
		if (put_arguments_in_list(*identity, command_line[i]) == 1)
		{
			separated_args = ft_split(command_line[i], '=');
			if (ft_strchr(separated_args[0], '+'))
			{
				conc = 1;
				while(separated_args[0][plus_finder])
				{
					if (separated_args[0][plus_finder] == '+')
						separated_args[0][plus_finder] = '\0';
					plus_finder++;
				}
			}
			insert_declaration(identity, conc, separated_args[0], separated_args[1]);
		}
	}
	free_matrix(separated_args);
}
