#include "minishell.h"

static bool	ft_check_var_name(const char	*str)
{
	while (str && *str)
	{
		if (ft_tolower(*str) == *str && *str != '_')
			return (false);
		++str;
	}
	return (true);
}

bool	ft_convert_str_to_struct(t_variable_env	*dst, const char	*src)
{
	char	*ptr;
	size_t	length;

	length = ft_strlen(src);
	ptr = ft_strnstr(src, "=", length);
	if (ptr == NULL || ptr == src)
		return (false);
	dst->name = ft_substr(src, 0, (size_t)((ptr - src) / sizeof(*src)));
	dst->value = ft_substr(src, (size_t)(ptr - src + 1), length - (size_t)(ptr - src + 1));
	if (dst->name == NULL || ft_check_var_name(dst->name) == false || dst->value == NULL)
		return (false);
	return (true);
}

static char	*ft_get_str(const t_variable_env	*var_env)
{
	char	*result;
	char	*backup;

	result = ft_strjoin(var_env->name, "=");
	backup = result;
	result = ft_strjoin(result, var_env->value);
	ft_smart_free((void **)&backup);
	return (result);
}

bool	ft_convert_vector_to_array(char	***dst,	const t_vector	*src)
{
	char			*str;
	size_t			i;

	if (dst == NULL)
		return (false);
	*dst = (char **)malloc((ft_size(src) + 1) * sizeof(**dst));
	if (*dst == NULL)
		return (false);
	i = 0;
	while (i < ft_size(src))
	{
		str = ft_get_str((const t_variable_env *)ft_get_element(src, i));
		if (str == NULL)
			return (false);
		(*dst)[i] = str;
		++i;
	}
	(*dst)[i] = NULL;
	return (true);
}
