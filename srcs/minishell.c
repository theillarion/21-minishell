#include "../includes/minishell.h"

extern char	**environ;

//		NOT WORK 
void	ft_free_double_array(char	***strs)
{
	if (strs == NULL || *strs == NULL)
		return ;
	while (**strs)
		free(*((*strs)++));
	free(*strs);
	*strs = NULL;
}

void	ft_main_handle(t_environment	*env)
{
	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
	if (env->input_line != NULL && ft_strlen(env->input_line) > 0)
	{
		add_history(env->input_line);
		if (preparse(env))
		{
			ft_init_vector(&env->tokens, sizeof(t_token));
			lexer(env);
			parser(env);

			int		status;
			pid_t	pid;
			pid = executor(env);
			if (pid)
			{
				if (waitpid(pid, &status, 0) == -1)
					ft_raise_error("waitpid error\n");
				if (WIFEXITED(status))
					exit(WEXITSTATUS(status));
			}
		}
	}
	if (env->is_need_update_envp)
	{
		env->is_need_update_envp = false;
		//ft_free_double_array(&env->envp);
		if (!ft_convert_vector_to_array(&env->envp, &env->variables_env))
			env->envp = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_environment	env;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_init(&env, envp, "\033[92mminishell\033[0m");
	if (sigaction(SIGQUIT, &env.action, NULL) == -1
		|| sigaction(SIGINT, &env.action, NULL) == -1)
	{
		ft_putendl_fd("Error!", 2);
	}
	ft_main_handle(&env);
	while (env.input_line)
	{
		ft_main_handle(&env);
	}
}
