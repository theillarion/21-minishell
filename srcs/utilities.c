#include "minishell.h"

void	ft_smart_free(void	**address)
{
	if (address == NULL || *address == NULL)
		return;
	free(*address);
	*address = NULL;
}

int	ft_smart_atoi(const char	*src, bool	*is_error)
{
	int	sign;
	int number;

	number = 0;
	sign = 1;
	while (*src == 32 || (*src > 8 && *src < 14))
		++src;
	if (*src == '-' || *src == '+')
	{
		if (*src == '-')
			sign = -1;
		++src;
	}
	while (ft_isdigit(*src))
		number = (number * 10) + (*(src++) - '0');
	if (*src != '\0')
		*is_error = true;
	return (number * sign);
}
