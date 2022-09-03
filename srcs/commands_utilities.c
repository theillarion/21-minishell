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

void ft_print_error(t_environment	*env, const char *command, const char *msg)
{
	if (env == NULL || command == NULL || msg == NULL)
		return ;
	ft_putstr_fd(env->info.name_shell, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd((char *)msg, STDERR_FILENO);
}
