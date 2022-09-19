#include "minishell.h"
#include <sys/ioctl.h>

typedef const char *const * t_double_ptr;

static void	execute(t_environment	*env)
{
	int		status;
	pid_t	pid;

	signal(SIGINT, ft_handle_signal_child);
	signal(SIGQUIT, ft_handle_signal_child);
	pid = executor(env);
	if (pid)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			if (errno != EINTR)
				ft_print_errno(env, "waitpid");
		}
		else if (WIFEXITED(status) != 0)
			env->last_code = WEXITSTATUS(status);
		postactions(env);
	}
}

void	ft_main_handle(t_environment	*env)
{
	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->variables_env, &env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
	if (env->input_line == NULL)
		ft_exit(env, 0);
	if (ft_strlen(env->input_line) > 0)
	{
		add_history(env->input_line);
		ft_init_vector(&env->tokens, sizeof(t_token), NULL);
		lexer(env);
		if (parser(env) == 0)
			execute(env);
	}
	if (env->is_need_update_envp)
	{
		env->is_need_update_envp = false;
		ft_smart_double_free((void ***)&env->envp);
		if (!ft_convert_vector_to_array(&env->envp, &env->variables_env))
			env->envp = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_environment	env;

	rl_catch_signals = 0;
	(void)argc;
	(void)argv;
	ft_init(&env);
	if (ft_fill(&env, envp, "\033[92mminishell\033[0m") == false)
		ft_exit_with_message(&env, COMMON_ERROR, NULL, "filling error");
	while (true)
    {
        signal(SIGINT, ft_handle_signal);
        signal(SIGQUIT, SIG_IGN);
        ft_main_handle(&env);
    }
	return (0);
}
