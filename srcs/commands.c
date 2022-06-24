#include "minishell.h"

void	ft_command_cd(t_environment 	*env, const char	*arg)
{
	if (env == NULL || chdir(arg) == -1)
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
	if (env == NULL || env->info.pwd == NULL)
		ft_error(env->info.name_shell, "pwd");
	else
		ft_putendl_fd(env->info.pwd, STDOUT_FILENO);
}

void	ft_command_env(const t_environment	*env)
{
	t_variable_env	var_env;
	size_t	i;

	if (env == NULL)
		return;
	i = 0;
	while (i < ft_size((const t_vector *)&env->variables_env))
	{
		var_env = *(t_variable_env *)ft_get_element(&env->variables_env, i);
		ft_putstr_fd(var_env.name, STDOUT_FILENO);
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd(var_env.value, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		++i;
	}
}

void	ft_command_unset(t_environment	*env, const char	*arg)
{
	size_t	index;

	if (env == NULL || arg == NULL)
		return;
	index = ft_find_by_name(&env->variables_env, arg);
	if (index < ft_size((const t_vector *)&env->variables_env))
		ft_erase(&env->variables_env, index);
}
