#include "../includes/minishell.h"

int	ft_command_exit(t_environment *env, const char	*arg)
{
	int		code;
	bool	is_error;

	if (env == NULL || arg == NULL)
		return (ft_exit(env, COMMON_ERROR, false));
	is_error = false;
	code = (unsigned int)ft_smart_atoi(arg, &is_error) % 256;
	if (is_error)
	{
		ft_putstr_fd(env->info.name_shell, STDERR_FILENO);
		ft_putstr_fd(": exit: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		return (ft_exit(env, 2, false));
	}
	else
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (ft_exit(env, code, false));
	}
}

int	ft_command_export(t_environment	*env, const char	*arg)
{
	t_variable_env	var_env;
	size_t			index;

	if (env != NULL && ft_convert_str_to_struct(&var_env, arg) == true)
	{
		index = ft_find_by_name(&env->variables_env, var_env.name);
		if (index < ft_size(&env->variables_env))
			ft_erase(&env->variables_env, index);
		ft_push(&env->variables_env, arg);
		env->is_need_update_envp = true;
	}
	return (SUCCESS);
}

int	ft_command_echo(t_environment	*env, const char	*arg)
{
	(void)env;
	(void)arg;
	return (0);
}
