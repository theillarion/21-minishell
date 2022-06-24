#include "minishell.h"

int	ft_exit(t_environment	*env, int status, bool is_clean)
{
	if (!is_clean)
		ft_deinit(env);
	exit(status);
}
