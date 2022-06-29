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

bool	ft_convert_to_struct(t_variable_env	*dst, const char	*src)
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
