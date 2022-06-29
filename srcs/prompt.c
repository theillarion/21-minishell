#include "minishell.h"

void	ft_set_new_prompt(t_prompt	*prompt, t_info	info)
{
	char	*backup;

	if (prompt == NULL)
	{
		//error
		return ;
	}
	backup = prompt->current_prompt;
	prompt->current_prompt = ft_strjoin(info.name_shell, ":\033[96m");
	ft_smart_free((void **)&backup);
	backup = prompt->current_prompt;
	prompt->current_prompt = ft_strjoin(prompt->current_prompt,
										info.pwd);
	ft_smart_free((void **)&backup);
	backup = prompt->current_prompt;
	prompt->current_prompt = ft_strjoin(prompt->current_prompt,
										"\033[0m->");
	ft_smart_free((void **)&backup);
	prompt->is_need_change = false;
}
