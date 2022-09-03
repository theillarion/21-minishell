#include "minishell.h"

int	add_redirect(t_vector *redirs, t_token *token, t_vector *tokens, size_t *i)
{
	t_redir	redir;
	t_token	*next_token;

	if (token->type != t_r_in && token->type != t_r_out
		&& token->type != t_r_outa && token->type != t_hd)
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

void	find_builtin(const t_environment *env, t_command *cmd, t_token *token)
{
	size_t		bi;
	char		*token_string;

	(*cmd).builtin = NULL;
	token_string = ft_substr(token->start, 0, token->size);
	bi = ft_find_by_name(&env->functions, token_string);
	if (bi != SIZE_MAX)
		(*cmd).builtin = ((t_function *)ft_get_element(&env->functions, bi));
	free(token_string);
}

void	process_word(t_environment *env, t_command *cmd, t_token *tkn, int *ca)
{
	if (tkn->type == t_word)
	{
		if (!*ca)
		{
			*ca = 1;
			(*cmd).command = tkn;
			find_builtin(env, cmd, tkn);
		}
		ft_push_back(&(*cmd).args, (void *)tkn);
	}
}

void	get_command(t_environment *env, size_t *i)
{
	t_command	cmd;
	t_token		*cur_token;
	int			command_added;

	ft_init_vector(&cmd.redirs, sizeof(t_redir));
	ft_init_vector(&cmd.args, sizeof(t_vector));
	command_added = 0;
	while (*i < ft_size(&env->tokens))
	{
		cur_token = (t_token *)ft_get_element(&env->tokens, *i);
		expand_word(env, &cur_token->start, &cur_token->size);
		if (cur_token->type == t_pipe)
			break ;
		add_redirect(&cmd.redirs, cur_token, &env->tokens, i);
		process_word(env, &cmd, cur_token, &command_added);
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
