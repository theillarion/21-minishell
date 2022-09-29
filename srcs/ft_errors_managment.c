/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors_managment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:09:52 by illarion          #+#    #+#             */
/*   Updated: 2022/09/29 15:13:46 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cmd_not_found(char *const *in_argv)
{
	ft_putstr_fd(in_argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

void	exit_find_failure(char **in_argv, char *access_denied_path)
{
	if (access_denied_path)
	{
		errno = EACCES;
		ft_raise_perror(access_denied_path, 1);
	}
	cmd_not_found(in_argv);
}

int	ft_raise_perror(char *strarg, int free_arg)
{
	perror(strarg);
	if (free_arg)
		free(strarg);
	exit(errno);
}

int	ft_raise_error(char *strarg)
{
	ft_putstr_fd(strarg, 2);
	exit(EXIT_FAILURE);
}
