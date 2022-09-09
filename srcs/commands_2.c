#include "minishell.h"

int	ft_command_exit(t_environment	*env, const char *const *args)
{
	int		code;
	bool	is_error;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (env == NULL || args == NULL || *args == NULL)
		return (ft_exit(env, SUCCESS));
	if (args != NULL && *args != NULL && *(args + 1) != NULL)
	{
		ft_print_error(env, "exit", "too many arguments");
		return (1);
	}
	is_error = false;
	code = (unsigned int)ft_smart_atoi(*args, &is_error) % 256;
	if (is_error)
	{
		ft_print_error(env, "exit", "numeric argument required");
		return (ft_exit(env, 2));
	}
	return (ft_exit(env, code));
}

int	ft_command_export(t_environment	*env, const char *const *args)
{
	t_variable_env	var_env;
	size_t			index;

	if (args == NULL)
		return (ft_command_env(env, args));
	while (*args)
	{
		if (ft_convert_str_to_struct(&var_env, *args))
		{
			index = ft_find_by_name(&env->variables_env, var_env.name);
			if (index < ft_size(&env->variables_env))
				ft_erase(&env->variables_env, index);
			ft_push_back(&env->variables_env, (void *)&var_env);
			env->is_need_update_envp = true;
		}
		++args;
	}
	return (SUCCESS);
}

int	ft_command_echo(t_environment	*env, const char *const *args)
{
	bool	is_need_newline;

	(void)env;
	is_need_newline = true;
	if (args && *args && ft_strlen(*args) == 2 && ft_strncmp(*args, "-n", 2) == 0)
	{
		is_need_newline  = false;
		++args;
	}
	while (args && *args)
	{
		ft_putstr_fd((char *)*args, STDOUT_FILENO);
		if (*(args + 1) != NULL)
			write(1, " ", 1);
		++args;
	}
	if (is_need_newline)
		write(1, "\n", 1);
	return (SUCCESS);
}
