#include "minishell.h"

bool ft_is_regular_file(char const *path)
{
	struct stat buff;

	if (stat(path, &buff) == 0 && S_ISREG(buff.st_mode))
		return (true);
	else
		return (false);
}

bool ft_is_exist(char const *path)
{
	if (access(path, X_OK) == 0)
		return (true);
	else
		return (false);
}
