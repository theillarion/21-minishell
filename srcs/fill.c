/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:59:16 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:00:09 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_fill_variables_env(t_vector	*vector, char	**envp)
{
	size_t	i;

	if (vector == NULL || envp == NULL)
		return ;
	i = 0;
	while (envp[i] != NULL)
		++i;
	ft_reserve(vector, i + 10);
	i = 0;
	while (envp[i] != NULL)
	{
		if (!ft_push(vector, envp[i]))
		{
			ft_clear_vector(vector);
			break ;
		}	
		++i;
	}
}

static void	ft_push_function(t_vector *src, char *name,
				int (*func)(t_environment *, const char *const *))
{
	t_function	result;

	result.name = name;
	result.func = func;
	ft_push_back(src, (void *)&result);
}

static void	ft_fill_functions(t_vector	*functions)
{
	ft_reserve(functions, COUNT_FUNCTIONS);
	ft_push_function(functions, "cd", &ft_command_cd);
	ft_push_function(functions, "pwd", &ft_command_pwd);
	ft_push_function(functions, "env", &ft_command_env);
	ft_push_function(functions, "unset", &ft_command_unset);
	ft_push_function(functions, "exit", &ft_command_exit);
	ft_push_function(functions, "export", &ft_command_export);
	ft_push_function(functions, "echo", &ft_command_echo);
}

static char	**ft_arraydup(char	**src)
{
	char	**result;
	size_t	i;

	if (!src)
		return (NULL);
	i = 0;
	while (src[i])
		++i;
	result = (char **)malloc((i + 1) * sizeof(*result));
	i = 0;
	while (src[i])
	{
		result[i] = ft_strdup(src[i]);
		++i;
	}
	result[i] = NULL;
	return (result);
}

bool	ft_fill(t_environment	*env, char	**envp, const char	*name_shell)
{
	if (!env || !envp || !*envp || !name_shell)
		return (false);
	ft_fill_variables_env(&env->variables_env, envp);
	ft_fill_functions(&env->functions);
	env->info.pwd = ft_get_pwd();
	env->info.name_shell = ft_strdup(name_shell);
	ft_set_new_prompt(&env->variables_env, &env->prompt, env->info);
	env->envp = ft_arraydup(envp);
	if (ft_size(&env->variables_env) != ft_size_array((void **)envp)
		|| ft_size(&env->functions) != COUNT_FUNCTIONS || !env->envp
		|| !env->info.pwd || !env->info.name_shell
		|| !env->prompt.current_prompt)
		return (false);
	return (true);
}
