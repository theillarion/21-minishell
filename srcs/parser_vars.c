/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:54 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 16:31:51 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_with_free(char *str1, char *str2, int free_1, int free_2)
{
	char	*result;

	if (! str1 && ! str2)
		return (NULL);
	else if (!str1)
		result = ft_strdup(str2);
	else if (!str2)
		result = ft_strdup(str1);
	else
		result = ft_strjoin(str1, str2);
	if (str1 && free_1)
	{
		free(str1);
		str1 = NULL;
	}
	if (str2 && free_2)
	{
		free(str2);
		str2 = NULL;
	}
	return (result);
}

char	*get_v(const char *string, int *i, const t_environment *env, int beg)
{
	char			*var_name;
	char			*var_value;
	size_t			var_index;
	t_variable_env	*var_element;
	char			**ptr;

	var_name = ft_substr(string, beg + 1, *i - beg - 1);
	var_index = ft_find_by_name(&env->variables_env, var_name);
	var_element = ft_get_element(&env->variables_env, var_index);
	var_value = NULL;
	if (var_element && var_element->values)
	{
		ptr = var_element->values;
		while (*ptr)
		{
			var_value = ft_strjoin_with_free(var_value, *ptr, 1, 0);
			if (*(ptr + 1))
				var_value = ft_strjoin_with_free(var_value, ":", 1, 0);
			ptr++;
		}
	}
	if (var_value && ft_strlen(var_name) > ft_strlen(var_value))
		*i = *i - (int)(ft_strlen(var_name) - ft_strlen(var_value));
	ft_smart_free((void *)&var_name);
	return (var_value);
}
