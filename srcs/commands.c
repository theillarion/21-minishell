#include "minishell.h"

int	ft_command_cd(t_environment	*env, const char *const *args)
{
	if (env == NULL || args == NULL || *args == NULL || chdir(*args) == -1)
	{
		ft_error(env->info.name_shell, "cd");
		return (COMMON_ERROR);
	}
	else
	{
		ft_smart_free((void **)&env->info.pwd);
		env->info.pwd = ft_get_pwd();
		env->prompt.is_need_change = true;
		return (SUCCESS);
	}
}

int	ft_command_pwd(t_environment	*env, const char *const *args)
{
	(void)args;
	if (env == NULL || env->info.pwd == NULL)
	{
		ft_error(env->info.name_shell, "pwd");
		return (COMMON_ERROR);
	}
	else
	{
		ft_putendl_fd(env->info.pwd, STDOUT_FILENO);
		return (SUCCESS);
	}
}

int	ft_command_env(t_environment	*env, const char *const *args)
{
	(void)args;
	if (env == NULL)
		return (COMMON_ERROR);
	while (env->envp && *env->envp)
	{
		ft_putendl_fd(*env->envp, STDOUT_FILENO);
		++env->envp;
	}
	return (SUCCESS);
}

int	ft_command_unset(t_environment	*env, const char *const *args)
{
	size_t	index;

	if (env == NULL || args == NULL)
		return (SUCCESS);
	while (*args)
	{
		index = ft_find_by_name(&env->variables_env, *args);
		if (index < ft_size((const t_vector *)&env->variables_env))
		{
			ft_erase(&env->variables_env, index);
			env->is_need_update_envp = true;
		}
		++args;
	}
	return (SUCCESS);
}
