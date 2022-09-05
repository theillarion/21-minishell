#include "minishell.h"

void	free_paths(char	**paths)
{
	char	*path;
	char	**paths_bkp;

	paths_bkp = paths;
	while (*paths_bkp)
	{
		path = *paths_bkp;
		free(path);
		paths_bkp++;
	}
	free(paths);
}

char	**find_path(char **envp)
{
	char	**env_path;

	env_path = envp;
	while (*env_path)
	{
		if (ft_strncmp(*env_path, "PATH=", 5))
			env_path = env_path + 1;
		else
		{
			*env_path = *env_path + 5;
			break ;
		}
	}
	if (*env_path == NULL)
		return (NULL);
	return (ft_split(*env_path, ':'));
}

void	ft_full_path(char **in_ar, char **paths, char **path, char **full_path)
{
	(*path) = ft_strjoin(*paths, "/");
	if (!(*path))
		exit(EXIT_FAILURE);
	(*full_path) = ft_strjoin((*path), in_ar[0]);
	if (!(*full_path))
	{
		free((*path));
		exit(EXIT_FAILURE);
	}
}

void	find_cmd_in_path(char **args, char **envp)
{
	char	**paths;
	char	*path;
	char	*full_path;
	char	*access_denied_path;
	char	**paths_bkp;

	paths = find_path(envp);
	if (!paths)
		cmd_not_found(args);
	access_denied_path = NULL;
	paths_bkp = paths;
	while (*paths)
	{
		ft_full_path(args, paths, &path, &full_path);
		execve(full_path, args, envp);
//		if (errno == EACCES)
//			access_denied_path = ft_strdup(full_path);
		free(full_path);
		free(path);
		paths++;
	}
	free_paths(paths_bkp);
	exit_find_failure(args, access_denied_path);
}
