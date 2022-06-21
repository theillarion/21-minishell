#include "minishell.h"

void	ft_get_line(t_environment	*env)
{
	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
}

int main(int argc, char **argv, char    **envp)
{
	t_environment	env;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_init(&env, "\033[92mminishell\033[0m");
	if (sigaction(SIGQUIT, &env.action, NULL) == -1
		|| sigaction(SIGINT, &env.action, NULL) == -1)
	{
		//error
		printf("Error!\n");
	}
	ft_get_line(&env);
	while (env.input_line)
	{
		printf("%s\n", env.input_line);
		ft_get_line(&env);
	}
}
