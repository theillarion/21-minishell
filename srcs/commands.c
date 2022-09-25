/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:51:59 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:52:00 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_command_cd(t_environment	*env, const char *const *args)
{
	char	**arg;

	if (!env || !args)
		return (SUCCESS);
	arg = (char **)args;
	if (!arg || !*arg || (**arg == '~' && *((*arg) + 1) == '\0'))
	{
		if (ft_find_by_name(&env->variables_env, "HOME") == SIZE_MAX)
			return (ft_print_error(env, "cd", "HOME not set"));
		arg = ft_get_by_name(&env->variables_env, "HOME")->values;
		if (*arg && *(arg + 1))
			return (ft_print_error(env, "cd", "No such file or directory"));
	}
	if (ft_strlen(*arg) == ft_strlen(env->info.pwd)
		&& ft_strncmp(*arg, env->info.pwd, ft_strlen(*arg)) == 0)
		return (SUCCESS);
	if (chdir(*arg) == -1)
		return (ft_print_errno(env, "chdir"));
	else
	{
		ft_smart_free((void **)&env->info.pwd);
		env->info.pwd = ft_get_pwd();
		env->prompt.is_need_change = true;
		return (SUCCESS);
	}
}

int	ft_command_pwd(t_environment	*env, const char *const *args)
{
	(void)args;
	if (env == NULL || env->info.pwd == NULL)
	{
		ft_print_error(env, "pwd", "invalid path");
		return (COMMON_ERROR);
	}
	else
	{
		ft_putendl_fd(env->info.pwd, STDOUT_FILENO);
		return (SUCCESS);
	}
}

static void	ft_print(void	*address)
{
	ft_putendl_fd((char *)address, STDOUT_FILENO);
}

int	ft_command_env(t_environment	*env, const char *const *args)
{
	(void)args;
	ft_foreach((void **)env->envp, &ft_print);
	return (SUCCESS);
}

int	ft_command_unset(t_environment	*env, const char *const *args)
{
	size_t	index;

	if (env == NULL)
		return (COMMON_ERROR);
	while (args && *args)
	{
		index = ft_find_by_name(&env->variables_env, *args);
		if (index < ft_size((const t_vector *)&env->variables_env))
		{
			ft_erase(&env->variables_env, index);
			env->is_need_update_envp = true;
		}
		++args;
	}
	return (SUCCESS);
}
