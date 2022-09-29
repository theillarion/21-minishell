/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:48:47 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:48:48 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_pwd(void)
{
	char	*path;

	path = (char *)malloc((PATH_MAX + 1) * sizeof(*path));
	ft_memset(path, '\0', PATH_MAX + 1);
	if (getcwd(path, PATH_MAX) == NULL)
	{
		ft_smart_free((void **)&path);
		return (NULL);
	}
	return (path);
}
