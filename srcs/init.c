#include "minishell.h"

static void	ft_fill_variables_env(t_vector	*vector, char	**envp)
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

static void ft_push_function(t_vector *src, char *name, int (*func)(t_environment *, const char *const *))
{
	t_function result;

	result.name = ft_strdup(name);
	result.func = func;
	ft_push_back(src, (void *)&result);
}

static void ft_fill_functions(t_environment *env)
{
	ft_push_function(&env->functions, "cd", &ft_command_cd);
	ft_push_function(&env->functions, "pwd", &ft_command_pwd);
	ft_push_function(&env->functions, "env", &ft_command_env);
	ft_push_function(&env->functions, "unset", &ft_command_unset);
	ft_push_function(&env->functions, "exit", &ft_command_exit);
	ft_push_function(&env->functions, "export", &ft_command_export);
	ft_push_function(&env->functions, "echo", &ft_command_echo);
}

void	ft_init(t_environment	*env, char	**envp,
				const char	*name_shell)
{
	if (env == NULL)
		return;
	ft_initial_action(&env->action);
	ft_init_vector(&env->variables_env, sizeof(t_variable_env));
	ft_init_vector(&env->functions, sizeof(t_function));
	ft_fill_variables_env(&env->variables_env, envp);
	ft_fill_functions(env);
	env->envp = envp;
	env->is_need_update_envp = false;
	env->info.pwd = ft_get_pwd();
	env->info.name_shell = ft_strdup(name_shell);
	env->prompt.current_prompt = NULL;
	env->prompt.is_need_change = false;
	env->input_line = NULL;
	env->last_code = 0;
	ft_set_new_prompt(&env->variables_env, &env->prompt, env->info);
}
