#include "minishell.h"

static char *ft_get_smart_pwd(const t_vector	*variable_env, const char *pwd)
{
	t_variable_env	*var;
	char			*result;
	char			**ptr;
	size_t			length;

	var = ft_get_by_name(variable_env, "HOME");
	if (!var)
		return (NULL);
	ptr = var->values;
	while (*ptr)
	{
		if (ft_strncmp(*ptr, pwd, ft_strlen(*ptr)) == 0)
			break ;
		++ptr;
	}
	if (!*ptr)
		return (NULL);
	length = ft_strlen(pwd) - ft_strlen(*ptr) + 1;
	result = (char *)malloc((length + 1) * sizeof(*result));
	result[0] = '~';
	ft_memcpy((void *)(result + 1),
		(const void *)(pwd + ft_strlen(*ptr)), length - 1);
	result[length] = '\0';
	return (result);
}

void	ft_set_new_prompt(const t_vector *variable_env, t_prompt	*prompt, t_info	info)
{
	char	*backup;
	char	*smart_pwd;

	backup = prompt->current_prompt;
	prompt->current_prompt = ft_strjoin(info.name_shell, ":\033[96m");
	ft_smart_free((void **)&backup);
	backup = prompt->current_prompt;
	smart_pwd = ft_get_smart_pwd(variable_env, info.pwd);
	if (smart_pwd)
		prompt->current_prompt = ft_strjoin(prompt->current_prompt, smart_pwd);
	else
		prompt->current_prompt = ft_strjoin(prompt->current_prompt, info.pwd);		
	ft_smart_free((void **)&backup);
	ft_smart_free((void **)&smart_pwd);
	backup = prompt->current_prompt;
	prompt->current_prompt = ft_strjoin(prompt->current_prompt,
										"\033[0m->");
	ft_smart_free((void **)&backup);
	prompt->is_need_change = false;
}
