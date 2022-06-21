#include "minishell.h"

void	ft_init(t_environment	*env, const char	*name_shell)
{
	env->info.pwd = ft_get_pwd();
	env->info.name_shell = ft_strdup(name_shell);
	env->prompt.is_need_change = false;
	ft_set_new_prompt(&env->prompt, env->info);
}
