/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abuonomo <abuonomo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 17:46:34 by lnicoter          #+#    #+#             */
/*   Updated: 2023/09/14 16:45:38 by abuonomo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_env(char **env, t_data *mini_data)
{
	int	env_size;
	int	i;

	env_size = 0;
	i = 0;
	while (env[env_size])
		env_size++;

	// Allocazione di spazio per env_size + 1 puntatori
	mini_data->copy_env = (char **)malloc((env_size + 1) * sizeof(char *));
	if (!mini_data->copy_env)
	{
		printf("Incombenza rilevata!!!\n");
		return (0); // Non c'è bisogno di liberare la memoria qui
	}

	while (i < env_size)
	{
		mini_data->copy_env[i] = ft_strdup(env[i]);
		i++;
	}
	mini_data->copy_env[i] = NULL; // Imposta l'ultimo puntatore a NULL

	return (1);
}

void *ft_realloc(void *ptr, size_t size)
{
	void *new_ptr;

	if (ptr)
	{
		if (size)
		{
			new_ptr = malloc(size);
			if (!new_ptr)
				return (NULL);
			ft_bzero(new_ptr, size);
			ft_memcpy(new_ptr, ptr, size);
		}
		else
		{
			new_ptr = (unsigned char *)malloc(sizeof(ptr));
			if (!new_ptr)
				return (NULL);
		}
		free(ptr);
		return (new_ptr);
	}
	return ((unsigned char *)malloc(sizeof(ptr) * size));
}

int	env_rows(t_data *shell_data)
{
	int	size;

	size = 0;
	while (shell_data->copy_env[size])
		size++;
	return (size);
}
