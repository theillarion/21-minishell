#include "minishell.h"

void ft_foreach(void **array, void (*func)(void *))
{
	if (!array || !func)
		return ;
	while (*array)
		(*func)(*(array++));
}

void	ft_smart_free(void	**address)
{
	if (address == NULL || *address == NULL)
		return;
	free(*address);
	*address = NULL;
}

void	ft_smart_double_free(void	***address)
{
	if (address && *address)
	{
		ft_foreach(*address, free);
		ft_smart_free((void **)address);
	}
}

int	ft_smart_atoi(const char	*src, bool	*is_error)
{
	int	sign;
	int number;

	number = 0;
	sign = 1;
	while (src != NULL && (*src == 32 || (*src > 8 && *src < 14)))
		++src;
	if (src != NULL && (*src == '-' || *src == '+'))
	{
		if (*src == '-')
			sign = -1;
		++src;
	}
	while (src != NULL && ft_isdigit(*src))
		number = (number * 10) + (*(src++) - '0');
	if (src == NULL || *src != '\0')
		*is_error = true;
	return (number * sign);
}

size_t	ft_size_array(void	**address)
{
	size_t	i;

	i = 0;
	while (address && address[i])
		++i;
	return (i);
}
