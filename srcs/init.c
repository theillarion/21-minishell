#include "minishell.h"

void	ft_init(t_environment	*env, const char	*name_shell)
{
	if (env == NULL)
		return;
	ft_initial_action(&env->action);
	env->info.pwd = ft_get_pwd();
	env->info.name_shell = ft_strdup(name_shell);
	env->prompt.current_prompt = NULL;
	env->prompt.is_need_change = false;
	env->input_line = NULL;
	ft_set_new_prompt(&env->prompt, env->info);
}
