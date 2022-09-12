#include "minishell.h"

void	ft_deinit(t_environment	*env)
{
	ft_smart_free((void **)&env->prompt.current_prompt);
	ft_smart_free((void **)&env->info.name_shell);
	ft_smart_free((void **)&env->info.pwd);
	ft_smart_free((void **)&env->input_line);
	ft_erase_all(&env->variables_env);
}
