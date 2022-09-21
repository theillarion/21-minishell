#include "minishell.h"

bool	ft_push(t_vector	*vector, const char	*string_var)
{
	t_variable_env	var;

	if (ft_convert_str_to_struct(vector, &var, string_var))
	{
		ft_push_back(vector, (void *)&var);
		return (true);
	}
	return (false);
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

t_variable_env *ft_get_by_name(const t_vector	*vector, const char *name)
{
	void	*ptr;
	size_t	find_index;

	find_index = ft_find_by_name(vector, name);
	if (find_index == SIZE_MAX)
		return (NULL);
	ptr = ft_get_element(vector, find_index);
	return ((t_variable_env *)ptr);
}
