/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abuonomo <abuonomo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:35:17 by abuonomo          #+#    #+#             */
/*   Updated: 2023/09/21 20:16:23 by abuonomo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ends_with(const char *str1, const char *str2) {
    int len1;
    int len2;

	len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
    if (len2 > len1)
        return 0;
    return ft_strcmp(str1 + (len1 - len2), str2) == 0;
}

char *extension(t_data *shell_data, const char *wildstring)
{
	char *ret;
	int i;

	i = 0;
	ret = ft_strdup("");
	while (shell_data->file[i] != NULL)
	{
		if (ends_with(shell_data->file[i]->d_name, wildstring))
			ret = ft_strjoin_wild(ret, shell_data->file[i]->d_name);
		i++;
	}
	return ret;
}

char *filename(t_data *shell_data, char *wildstring)
{
	char *ret;
	int i;

	ret = ft_strdup("");
	i = 0;
	while (shell_data->file[i] != NULL)
	{
		if (ft_strncmp(shell_data->file[i]->d_name, wildstring, ft_strlen(wildstring)) == 0)
			ret = ft_strjoin_wild(ret, shell_data->file[i]->d_name);
		i++;
	}
	return (ret);
}

char *partial(t_data *shell_data, char *wildstring)
{
	int i;
	char *ret;

	i = 0;
	ret = ft_strdup("");
	while (shell_data->file[i] != NULL)
	{
		if (shell_data->file[i]->d_name[0] != '.')
		{
			if (ft_strnstr(shell_data->file[i]->d_name, wildstring, strlen(shell_data->file[i]->d_name)) != NULL)
				ret = ft_strjoin_wild(ret, shell_data->file[i]->d_name);
		}
		i++;
	}
	return (ret);
}

char *translate_wild(t_data *shell_data, char *input)
{
	char *wildstring;
	int i;
	int x;

	i = 0;
	x = 0;
	wildstring = malloc(ft_strlen(input) + 1);

	if (ft_isalpha(input[i]))
	{
		while (input[i] != '*')
			wildstring[x++] = input[i++];
		wildstring[x] = 0;
		return filename(shell_data, wildstring);
	}
	else if (input[i] == '*')
	{
		i++;
		while (ft_isalpha(input[i]) || input[i] == '.')
			wildstring[x++] = input[i++];
		wildstring[x] = 0;
		if (input[i] == '*')
			return partial(shell_data, wildstring);
		return extension(shell_data, wildstring);
	}
	return NULL;
}
