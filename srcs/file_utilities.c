/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utilities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 23:03:34 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:04:22 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	may be unused
bool	ft_is_regular_file(char const *path)
{
	struct stat	buff;

	if (stat(path, &buff) == 0 && S_ISREG(buff.st_mode))
		return (true);
	else
		return (false);
}

//	may be unused
bool	ft_is_exist(char const *path)
{
	if (access(path, X_OK) == 0)
		return (true);
	else
		return (false);
}

static char	*ft_get_directory(const char *path, const char *name)
{
	char	*result;
	char	*backup;

	result = (char *)path;
	backup = NULL;
	if (path[ft_strlen(path) - 1] != '/')
	{
		result = ft_strjoin(path, "/");
		backup = result;
	}
	result = ft_strjoin(result, name);
	ft_smart_free((void **)&backup);
	return (result);
}

//	may be unused
bool	ft_which(const char *const *paths, const char *name, char **dst)
{
	char	*buff;

	*dst = NULL;
	buff = NULL;
	while (paths && *paths)
	{
		buff = ft_get_directory(*paths, name);
		if (ft_is_exist(buff) && ft_is_regular_file(buff))
			break ;
		ft_smart_free((void **)&buff);
		++paths;
	}
	if (buff)
	{
		*dst = buff;
		return (true);
	}
	return (false);
}
