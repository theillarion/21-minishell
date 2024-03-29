/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 23:04:33 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:04:55 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_error(const t_environment	*env, const char *command,
			const char *msg)
{
	if (env && env->info.name_shell)
	{
		ft_putstr_fd(env->info.name_shell, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (command)
	{
		ft_putstr_fd((char *)command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (msg)
		ft_putendl_fd((char *)msg, STDERR_FILENO);
	return (COMMON_ERROR);
}

int	ft_print_errno(t_environment	*env, const char *command)
{
	if (errno != 0)
	{
		env->last_code = errno;
		ft_print_error(env, command, strerror(errno));
		errno = 0;
		return (COMMON_ERROR);
	}
	return (SUCCESS);
}
