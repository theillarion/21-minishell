#include "minishell.h"

void	ft_command_cd(t_environment 	*env, const char	*arg)
{
	if (chdir(arg) == -1)
		ft_error(env->info.name_shell, "cd");
	else
	{
		ft_smart_free((void **)&env->info.pwd);
		env->info.pwd = ft_get_pwd();
		env->prompt.is_need_change = true;
	}
}

char	*ft_get_pwd()
{
	char	*path;

	path = (char *)malloc(101 * sizeof(*path));
	ft_memset(path, 0, 101);
	if (getcwd(path, 100) == NULL)
	{
		free(path);
		return (NULL);
	}
	return (path);
}

void	ft_command_pwd(t_environment	*env)
{
	if (env->info.pwd == NULL)
		ft_error(env->info.name_shell, "pwd");
	else
		ft_putendl_fd(env->info.pwd, STDOUT_FILENO);
}
