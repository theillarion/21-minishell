#include "minishell.h"

int	add_redirect(t_vector *redirs, t_token *token, t_vector *tokens, size_t *i)
{
	t_redir	redir;
	t_token	*next_token;

	if (token->type != t_r_in && token->type != t_r_out && token->type != t_r_outa && token->type != t_hd)
		return (0);
	(*i)++;
	next_token = (t_token *)ft_get_element(tokens, *i);
	if (!next_token)
		return (0);
	redir.arg = next_token;
	redir.r_type = token->type;
	ft_push_back(redirs, (void *)&redir);
	return (1);
}

void	get_command(t_environment *env, size_t *i)
{
	t_command	cmd;
	t_token		*cur_token;
	int			com_added;

	ft_init_vector(&cmd.redirs, sizeof(t_redir));
	ft_init_vector(&cmd.args, sizeof(t_vector));
	cmd.builtin = NULL;
	com_added = 0;
	while (*i < ft_size(&env->tokens))
	{
		cur_token = (t_token *)ft_get_element(&env->tokens, *i);
		expand_word(env, &cur_token->start, &cur_token->size);
		if (cur_token->type == t_pipe)
			break ;
		add_redirect(&cmd.redirs, cur_token, &env->tokens, i);
		if (cur_token->type == t_word)
		{
			if (!com_added)
			{
				com_added = 1;
				cmd.command = cur_token;
				// find builtin
				size_t	index;

				index = ft_find_by_name(&env->functions, ft_substr(cur_token->start, 0, cur_token->size));
				if (index != SIZE_MAX)
					cmd.builtin = ((t_function *)ft_get_element(&env->functions, index));
			}
			ft_push_back(&cmd.args, (void *)cur_token);
		}
		(*i)++;
	}
	ft_push_back(&env->groups, (void *)&cmd);
}

void	parser(t_environment *env)
{
	size_t		i;

	ft_init_vector(&env->groups, sizeof(t_command));
	i = -1;
	while (++i < ft_size(&env->tokens))
		get_command(env, &i);
}
