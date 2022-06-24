#include "minishell.h"

int	ft_command_exit(t_environment 	*env, const char	*arg)
{
	int		code;
	bool	is_error;

	is_error = false;
	code = (unsigned int)ft_smart_atoi(arg, &is_error) % 256;
	if (is_error)
	{
		ft_putstr_fd(env->info.name_shell, STDERR_FILENO);
		ft_putstr_fd(": exit: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		ft_exit(env, 2, false);
		return (2);
	}
	else
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		ft_exit(env, code, false);
		return (code);
	}
}
