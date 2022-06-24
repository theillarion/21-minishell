#include "minishell.h"

void	ft_fill_vector(t_vector	*vector, const char	**envp)
{
	size_t	i;

	if (vector == NULL || envp == NULL)
		return;
	i = 0;
	while (envp[i] != NULL)
		++i;
	ft_reserve(vector, i + 10);
	i = 0;
	while (envp[i] != NULL)
	{
		ft_push(vector, envp[i]);
		++i;
	}
}

void	ft_init(t_environment	*env, const char	**envp,
				const char	*name_shell)
{
	if (env == NULL)
		return;
	ft_initial_action(&env->action);
	ft_init_vector(&env->variables_env, sizeof(t_variable_env));
	ft_fill_vector(&env->variables_env, envp);
	env->info.pwd = ft_get_pwd();
	env->info.name_shell = ft_strdup(name_shell);
	env->prompt.current_prompt = NULL;
	env->prompt.is_need_change = false;
	env->input_line = NULL;
	env->last_code = 0;
	ft_set_new_prompt(&env->prompt, env->info);
}
