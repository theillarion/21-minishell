/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_variable_env.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:53:28 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:57:09 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_calc_length(const t_variable_env	*var_env)
{
	size_t	result;
	char	**ptr;

	result = 0;
	if (var_env == NULL)
		return (result);
	result = ft_strlen(var_env->name) + 1;
	ptr = var_env->values;
	while (*ptr)
		result += ft_strlen(*(ptr++));
	if (ptr != var_env->values)
		result += (size_t)((ptr - var_env->values)) - 1;
	return (result);
}

char	*ft_get_str(const t_variable_env	*var_env)
{
	char	*result;
	char	**ptr;
	size_t	length;
	size_t	i;

	length = ft_calc_length(var_env);
	result = (char *)malloc((length + 1) * sizeof(*result));
	i = ft_strlen(var_env->name);
	ft_memcpy(result, var_env->name, sizeof(*var_env->name) * i);
	result[i++] = '=';
	ptr = var_env->values;
	while (*ptr)
	{
		length = ft_strlen(*ptr);
		ft_memcpy(result + i, *ptr, sizeof(**ptr) * length);
		i += length;
		if (*(ptr + 1))
			result[i++] = ':';
		++ptr;
	}
	result[i] = '\0';
	return (result);
}

bool	ft_convert_vector_to_array(char	***dst,	const t_vector	*src)
{
	size_t	i;
	size_t	length;

	if (dst == NULL)
		return (false);
	length = ft_size(src);
	*dst = (char **)malloc((length + 1) * sizeof(**dst));
	if (*dst == NULL)
		return (false);
	i = 0;
	while (i < length)
	{
		(*dst)[i] = ft_get_str((const t_variable_env *)ft_get_element(src, i));
		if ((*dst)[i] == NULL)
			return (false);
		++i;
	}
	(*dst)[i] = NULL;
	return (true);
}
