#include "minishell.h"

int	ft_command_cd(t_environment	*env, const char *const *args)
{
	char	*arg;

	if (!env || !args)
		return (SUCCESS);
	arg = (char *)*args;
	if (!arg || (*arg == '~' && *(arg + 1) == '\0'))
		arg = *(ft_get_by_name(&env->variables_env, "HOME")->values);
	if (ft_strlen(arg) == ft_strlen(env->info.pwd)
		&& ft_strncmp(arg, env->info.pwd, ft_strlen(arg)) == 0)
		return (SUCCESS);
	if (chdir(arg) == -1)
	{
		ft_print_errno(env, "chdir");
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
