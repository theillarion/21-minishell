#include "minishell.h"

void	ft_smart_free(void	**address)
{
	if (address == NULL || *address == NULL)
		return;
	free(*address);
	*address = NULL;
}
