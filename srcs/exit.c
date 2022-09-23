/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:52:53 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:52:54 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_environment	*env, int status)
{
	ft_destroy(env);
	exit(status);
	return (status);
}

void	ft_exit_with_message(t_environment	*env, int status,
			const char	*command, const char	*msg)
{
	ft_print_error(env, command, msg);
	ft_exit(env, status);
}
