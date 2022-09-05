#include "minishell.h"

char	*ft_get_pwd(void)
{
	char	*path;

	path = (char *)malloc((PATH_MAX + 1) * sizeof(*path));
	ft_memset(path, '\0', PATH_MAX + 1);
	if (getcwd(path, PATH_MAX) == NULL)
	{
		free(path);
		return (NULL);
	}
	return (path);
}
