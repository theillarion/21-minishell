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

static bool	ft_convert_to_struct(t_variable_env	*dst, const char	*src)
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

void	ft_push(t_vector	*vector, const char	*string_var)
{
	t_variable_env	var;

	if (ft_convert_to_struct(&var, string_var) == false)
	{
		// error
		return;
	}
	ft_push_back(vector, (void *)&var);
}

size_t	ft_find_by_name(const t_vector	*vector, const char	*name)
{
	t_variable_env	var_env;
	size_t			i;

	i = 0;
	while (i < ft_size(vector))
	{
		var_env = *(t_variable_env *)ft_get_element(vector, i);
		if (var_env.name == NULL
			|| (ft_strlen(var_env.name) == ft_strlen(name)
			&& ft_strncmp(var_env.name, name, ft_strlen(var_env.name)) == 0))
			return (i);
		++i;
	}
	return (SIZE_MAX);
}
