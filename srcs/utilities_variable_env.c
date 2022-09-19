#include "minishell.h"

static bool	ft_check_var_name(const char	*str)
{
	if (str == NULL || ft_isdigit(*str) == 1)
		return (false);
	return (true);
}

bool	ft_convert_str_to_struct(t_variable_env	*dst, const char	*src)
{
	char	*ptr;
	char	*value;
	size_t	length;

	length = ft_strlen(src);
	ptr = ft_strnstr(src, "=", length);
	if (ptr == NULL || ptr == src)
		return (false);
	dst->name = ft_substr(src, 0, (size_t)((ptr - src) / sizeof(*src)));
	value = ft_substr(src, (size_t)(ptr - src + 1), length - (size_t)(ptr - src + 1));
	if (dst->name == NULL || ft_check_var_name(dst->name) == false || value == NULL)
	{
		ft_smart_free((void **)&value);
		return (false);
	}
	dst->values = ft_split(value, ':');
	ft_smart_free((void **)&value);
	if (dst->values == NULL)
		return (false);
	return (true);
}

static size_t ft_calc_length(const t_variable_env	*var_env)
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

static char	*ft_get_str(const t_variable_env	*var_env)
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

bool	ft_convert_token_vector_to_str_array(char	***dst,	const t_vector	*src)
{
	char			*str;
	size_t			i;
	t_token 		*token;

	if (dst == NULL)
		return (false);
	*dst = (char **)malloc((ft_size(src) + 1) * sizeof(**dst));
	if (*dst == NULL)
		return (false);
	i = -1;
	int j=0;

	while (++i < ft_size(src))
	{
		token = (t_token *)ft_get_element(src, i);
		if (token->type == t_sep)
			continue ;
		str = NULL;
		while (token->type != t_sep && i < ft_size(src))
		{
			str = ft_strjoin(str, ft_substr(token->start, 0, token->size));
			i++;
			token = (t_token *)ft_get_element(src, i);
		}

		if (str == NULL)
			return (false);
		(*dst)[j] = str;
		j++;
	}
	(*dst)[j] = NULL;
	return (true);
}
