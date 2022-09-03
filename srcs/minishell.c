#include "minishell.h"

typedef const char *const * t_double_ptr;

void	ft_execute_external(t_environment	*env, char **args)
{
	pid_t	pid;
	int		status;
	char	*program;

	pid = fork();
	if (pid < 0)
		ft_error(env->info.name_shell, *args);
	else if (pid == 0)
	{
		if (ft_which((t_double_ptr)ft_get_by_name(&env->variables_env, "PATH")->values, *args, &program))
			status = execve(program, args, env->envp);
		else
			status = execve(*args, args, env->envp);
		ft_smart_free((void **)&program);
		if (status < 0)
			ft_error(env->info.name_shell, *args);
		exit(status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) != 0)
			env->last_code = WEXITSTATUS(status);
		else
			ft_print_error(env, *args, "error");
	}
}

void	ft_exucute_program(t_environment	*env, char **args)
{
	size_t index;

	index = ft_find_by_name(&env->functions, *args);
	if (index == SIZE_MAX)
		ft_execute_external(env, args);
	else
		(*(t_function *)ft_get_element(&env->functions, index)).func(env, (const char *const *)++args);
	
}

void	ft_main_handle(t_environment	*env)
{
	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
	if (env->input_line != NULL && ft_strlen(env->input_line) > 0)
		add_history(env->input_line);
	
	/*
	 *	..Handle...
	*/

	// TEMP
	char **temp = ft_split(env->input_line, ' ');
	ft_exucute_program(env, temp);
	//ft_free_double_array(&temp);	

	/*
	 *	...Execute program..
	*/
	if (env->is_need_update_envp)
	{
		env->is_need_update_envp = false;
		//ft_free_double_array(&env->envp);
		if (!ft_convert_vector_to_array(&env->envp, &env->variables_env))
			env->envp = NULL;
	}
}

int main(int argc, char **argv, char    **envp)
{
	t_environment	env;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_init(&env, envp, "\033[92mminishell\033[0m");
	if (sigaction(SIGQUIT, &env.action, NULL) == -1
		|| sigaction(SIGINT, &env.action, NULL) == -1)
	{
		//error
		printf("Error!\n");
	}

	ft_main_handle(&env);
	while (env.input_line)
	{
		ft_main_handle(&env);
	}
}
