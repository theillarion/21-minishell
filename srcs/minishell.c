#include "../includes/minishell.h"

void	ft_get_line(t_environment	*env)
{
	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
	if (env->input_line != NULL && ft_strlen(env->input_line) > 0)
	{
		add_history(env->input_line);
	}
}

int main(int argc, char **argv, char    **envp)
{
	t_environment	env;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_init(&env, (const char **)envp, "\033[92mminishell\033[0m");
	if (sigaction(SIGQUIT, &env.action, NULL) == -1
		|| sigaction(SIGINT, &env.action, NULL) == -1)
	{
		//error
		printf("Error!\n");
	}
	ft_get_line(&env);
//	ft_command_env(&env);
	while (env.input_line)
	{
		if (preparse(&env))
		{
			ft_init_vector(&env.tokens, sizeof(t_token));
			lexer(&env);
			parser(&env);

			int			status;
			pid_t		pid;
			pid = executor(&env);
			if (pid)
			{
				if (waitpid(pid, &status, 0) == -1)
					ft_raise_error("waitpid error\n");
//				if (WIFEXITED(status))
//					exit(WEXITSTATUS(status));
			}
		}
		ft_get_line(&env);
		//ft_command_unset(&env, env.input_line);
//		ft_command_env(&env);
	}
}
