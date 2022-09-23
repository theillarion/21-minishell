/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_variable_env_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:59:06 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:59:06 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_add_old_var(const t_vector *variable_env, char **value,
				const char *name)
{
	t_variable_env	*var;
	size_t			size;
	char			*result;
	char			*str;
	char			*ptr;

	var = ft_get_by_name(variable_env, name);
	if (var == NULL)
		return (*value);
	str = ft_get_str(var);
	size = ft_strlen(str) - 1 - ft_strlen(var->name) + ft_strlen(*value);
	result = (char *)malloc((size + 1) * sizeof(*result));
	if (result == NULL)
		return (NULL);
	ft_memset(result, '\0', (size + 1) * sizeof(*result));
	size = ft_strlen(str);
	ptr = ft_strnstr(str, "=", size);
	size = ft_strlen(ptr + 1);
	ft_memcpy(result, ptr + 1, size * sizeof(*ptr));
	ft_memcpy(result + size, *value, ft_strlen(*value) * sizeof(**value));
	ft_smart_free((void **)&str);
	ft_smart_free((void **)value);
	return (result);
}

static bool	ft_check_var_name(const char	*str)
{
	if (str == NULL || ft_isdigit(*str))
		return (false);
	if (ft_isalpha(*str) == 0 && *str != '_')
		return (false);
	while (*(++str))
	{
		if (!(ft_isalpha(*str) || ft_isdigit(*str)
				|| *str == '_' || (!*(str + 1) && *str == '+')))
			return (false);
	}
	return (true);
}

bool	ft_convert_str_to_struct(const t_vector	*variable_env,
			t_variable_env	*dst, const char	*src)
{
	char			*ptr;
	size_t			i;
	char			*value;
	size_t			length;

	i = 0;
	length = ft_strlen(src);
	ptr = ft_strnstr(src, "=", length);
	if (ptr == NULL || ptr == src)
		return (false);
	if (*(ptr - 1) == '+')
		i = 1;
	dst->name = ft_substr(src, 0, ((size_t)((ptr - src) - i) / sizeof(*src)));
	value = ft_substr(src, (size_t)(ptr - src + 1),
			length - (size_t)(ptr - src + 1));
	if (i == 1)
		value = ft_add_old_var(variable_env, &value, dst->name);
	dst->values = ft_smart_split(value, ':', true);
	ft_smart_free((void **)&value);
	if (!dst->name || !ft_check_var_name(dst->name) || !dst->values)
		return (false);
	return (true);
}
