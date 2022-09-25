#include "minishell.h"

void	free_command_args(char **args)
{
	char	**args_bkp;

	args_bkp = args;
	while (*args_bkp)
	{
		free(*args_bkp);
		args_bkp++;
	}
	free(args);
}

void	ft_free_groups(t_environment *env)
{
	size_t		i;
	t_command	*current;
	t_token		*token;

	i = -1;
	while (++i < ft_size(&env->groups))
	{
		current = ft_get_element(&env->groups, i);
		ft_erase_all(&current->args);
		ft_erase_all(&current->redirs);
	}
	ft_erase_all(&env->groups);
	i = -1;
	while (++i < ft_size(&env->tokens))
	{
		token = ft_get_element(&env->tokens, i);
		free(token->start);
		if (token->type == t_r_out || token->type == t_r_outa
			|| token->type == t_r_in || token->type == t_hd)
		{
			++i;
			token = ft_get_element(&env->tokens, i);
			if (token->type == t_sep)
				i++;
		}
	}
	ft_erase_all(&env->tokens);
}

void	postactions(t_environment *env)
{
	ft_free_groups(env);
}
