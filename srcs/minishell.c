/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 23:21:32 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:27:37 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/ioctl.h>

typedef const char *const	*t_double_ptr;

static void	ft_execute_external(t_environment	*env, char **args)
{
	char	*program;
	int		status;

	if (ft_which((t_double_ptr)ft_get_by_name(&env->variables_env, "PATH")
			->values, *args, &program))
		status = execve(program, args, env->envp);
	else
		status = execve(*args, args, env->envp);
	ft_smart_free((void **)&program);
	if (status < 0)
		ft_print_errno(env, *args);
	exit(status);
}

static void	ft_fork(t_environment	*env, char **args)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, ft_handle_signal_child);
	signal(SIGQUIT, ft_handle_signal_child);
	pid = fork();
	if (pid < 0)
		ft_print_errno(env, "fork");
	else if (pid == 0)
		ft_execute_external(env, args);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			if (errno != EINTR)
				ft_print_errno(env, "waitpid");
		}
		else if (WIFEXITED(status) != 0)
			env->last_code = WEXITSTATUS(status);
	}
}

void	ft_exucute_program(t_environment	*env, char **args)
{
	size_t	index;

	index = ft_find_by_name(&env->functions, *args);
	if (index == SIZE_MAX)
		ft_fork(env, args);
	else
		((t_function *)ft_get_element(&env->functions, index))->func(env,
			(t_double_ptr)++args);
}

void	ft_main_handle(t_environment	*env)
{
	char	**temp;

	if (env->prompt.is_need_change == true)
		ft_set_new_prompt(&env->variables_env, &env->prompt, env->info);
	ft_smart_free((void **)&env->input_line);
	env->input_line = readline(env->prompt.current_prompt);
	if (env->input_line == NULL)
		ft_exit(env, 0);
	if (ft_strlen(env->input_line) > 0)
		add_history(env->input_line);
	temp = ft_split(env->input_line, ' ');
	if (temp && *temp)
	{
		ft_exucute_program(env, temp);
		ft_smart_double_free((void ***)&temp);
	}
	if (env->is_need_update_envp)
	{
		env->is_need_update_envp = false;
		ft_smart_double_free((void ***)&env->envp);
		if (!ft_convert_vector_to_array(&env->envp, &env->variables_env))
			env->envp = NULL;
	}
}

int	main(int argc, char	**argv, char	**envp)
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
