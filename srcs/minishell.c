#include "minishell.h"

extern char **environ;

//		NOT WORK 
void	ft_free_double_array(char	***strs)
{
	if (strs == NULL || *strs == NULL)
		return;
	while (**strs)
		free(*((*strs)++));
	free(*strs);
	*strs = NULL;
}

[[maybe_unused]] static char *ft_get_directory(const char *path, const char *name)
{
	char	*result;
	char	*backup;

	result = (char *)path;
	backup = NULL;
	if (path[ft_strlen(path) - 1] != '/')
	{
		result = ft_strjoin(path, "/");
		backup = result;
	}
	result = ft_strjoin(result, name);
	ft_smart_free((void **)&backup);
	return (result);
}

void	ft_exucute_program(t_environment	*env, char **arg)
{
	size_t index;

	index = ft_find_by_name(&env->functions, *arg);
	if (index == SIZE_MAX)
		printf("Function not found\n");
	else
		(*(t_function *)ft_get_element(&env->functions, index)).func(env, *(++arg));
	// pid_t	pid;
	// int		status;

	// pid = fork();
	// if (pid < 0)
	// {
	// 	ft_error(env->info.name_shell, arg[0]);
	// }
	// else if (pid == 0)
	// {
	// 	bool is_find;
	// 	char	**ptr;
	// 	char	*program;

	// 	ptr = (*(t_variable_env *)ft_get_element(&env->variables_env,
	// 		ft_find_by_name(&env->variables_env, "PATH"))).values;
	// 	is_find = false;
	// 	while (ptr && *ptr && !is_find)
	// 	{
	// 		program = ft_get_directory(*ptr, *arg);
	// 		if (ft_is_exist(program) && ft_is_regular_file(program))
	// 			is_find = true;
	// 		++ptr;
	// 	}
	// 	if (is_find)
	// 		status = execve(program, arg, env->envp);
	// 	else
	// 		status = execve(*arg, arg, env->envp);
	// 	if (status < 0)
	// 		ft_error(env->info.name_shell, arg[0]);
	// 	exit(status);
	// }
	// else
	// {
	// 	waitpid(pid, &status, 0);
	// 	if (WIFEXITED(status) != 0)
	// 		env->last_code = WEXITSTATUS(status);
	// }
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
