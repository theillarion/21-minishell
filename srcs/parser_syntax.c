#include "minishell.h"

int	ft_syntax_error(t_environment *env)
{
	return (ft_print_error(env, NULL, "syntax error\n"));
}

void	skip_t_sep(t_environment *env, t_token **current_token, size_t *i)
{
	while ((*current_token)->type == t_sep)
		(*current_token) = (t_token *) ft_get_element(&env->tokens, ++(*i));
}

int	check_syntax_token(t_environment *env, size_t *i)
{
	t_token			*last_token;
	t_token			*current_token;
	enum e_Token	lt;
	enum e_Token	ct;

	current_token = (t_token *) ft_get_element(&env->tokens, *i);
	ct = current_token->type;
	if (1 <= *i)
	{
		last_token = (t_token *) ft_get_element(&env->tokens, *i - 1);
		lt = last_token->type;
		if ((lt == t_r_in || lt == t_r_out || lt == t_r_outa || lt == t_hd)
			&& (ct == t_sep))
			skip_t_sep(env, &current_token, i);
		ct = current_token->type;
		if ((lt == t_r_in || lt == t_r_out || lt == t_r_outa || lt == t_hd)
			&& (ct != t_word && ct != t_sep))
			return (ft_syntax_error(env));
	}
	if (*i + 1 == ft_size(&env->tokens) && (ct != t_word && ct != t_sep))
		return (ft_syntax_error(env));
	return (0);
}
