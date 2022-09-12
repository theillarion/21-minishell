#include "minishell.h"

void	ft_delete_element(void	*address)
{
	t_variable_env	*var_env;

	var_env = (t_variable_env *)address;
	ft_smart_free((void **)&var_env->name);
	ft_smart_double_free((void ***)&var_env->values);
}

void	ft_init(t_environment	*env)
{
	if (env == NULL)
		return;
	ft_init_vector(&env->variables_env, sizeof(t_variable_env), &ft_delete_element);
	ft_init_vector(&env->functions, sizeof(t_function), NULL);
	ft_init_action(&env->action);
	env->info.name_shell = NULL;
	env->info.pwd = NULL;
	env->prompt.current_prompt = NULL;
	env->prompt.is_need_change = false;
	env->input_line = NULL;
	env->envp = NULL;
	env->is_need_update_envp = false;
	env->last_code = 0;
}
