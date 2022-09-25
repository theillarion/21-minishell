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
	if (next_token->type == t_sep)
		next_token = (t_token *)ft_get_element(tokens, ++(*i));
	if (!next_token)
		return (0);
	redir.arg = next_token;
	redir.r_type = token->type;
	ft_push_back(redirs, (void *)&redir);
	return (1);
}

void	find_builtin(const t_environment *env, t_cmd *cmd, t_token *token)
{
	size_t		bi;
	char		*token_string;

	if (!token->size)
		return ;
	token_string = ft_substr(token->start, 0, token->size);
	bi = ft_find_by_name(&env->functions, token_string);
	if (bi != SIZE_MAX)
		(*cmd).builtin = ((t_function *)ft_get_element(&env->functions, bi));
	free(token_string);
}

void	add_cmd_and_args(t_environment *env, t_cmd *cmd, t_token *tkn, int *ca)
{
	if (tkn->type == t_word || tkn->type == t_sep)
	{
		if (!*ca && tkn->type == t_word)
		{
			*ca = 1;
			(*cmd).command = tkn;
			find_builtin(env, cmd, tkn);
		}
		ft_push_back(&(*cmd).args, (void *)tkn);
	}
}

int	get_command(t_environment *env, size_t *i)
{
	t_cmd	cmd;
	t_token	*cur_token;
	int		command_added;

	cmd.builtin = NULL;
	ft_init_vector(&cmd.redirs, sizeof(t_redir), NULL);
	ft_init_vector(&cmd.args, sizeof(t_token), NULL);
	command_added = 0;
	while (*i < ft_size(&env->tokens))
	{
		cur_token = (t_token *)ft_get_element(&env->tokens, *i);
		if (expand_word(env, &cur_token->start, &cur_token->size))
			return (1);
		if (cur_token->type == t_pipe)
			break ;
		add_redirect(&cmd.redirs, cur_token, &env->tokens, i);
		add_cmd_and_args(env, &cmd, cur_token, &command_added);
		(*i)++;
	}
	ft_push_back(&env->groups, (void *)&cmd);
	return (0);
}

int	parser(t_environment *env)
{
	size_t		i;

	ft_init_vector(&env->groups, sizeof(t_cmd), NULL);
	i = -1;
	while (++i < ft_size(&env->tokens))
	{
		if (check_syntax_token(env, i))
			return (1);
	}
	i = -1;
	while (++i < ft_size(&env->tokens))
	{
		if (get_command(env, &i))
			return (1);
	}
	return (0);
}
