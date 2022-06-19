#include "minishell.h"

void	ft_readline_insert(const char	*str)
{
	char	*backup;

	backup = rl_line_buffer;
	rl_line_buffer = ft_strjoin(rl_line_buffer, str);
	ft_smart_free((void **)&backup);
	rl_replace_line(rl_line_buffer, 0);
	rl_redisplay();
}
