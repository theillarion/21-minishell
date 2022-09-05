#include "minishell.h"

int	preparse(t_environment *env)
{
	char	*il;

	// ToDo Write checks for bash syntax
	il = env->input_line;
	while (*il)
	{
		il++;
	}
	return (1);
}

void	double_sized_token(char **input, char *res, t_token *ttoken, char type)
{
	(*res) = type;
	(*ttoken).size = 2;
	*input = *input + 1;
}

void	parse_token(char **input, t_token *ttoken)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
			double_sized_token(input, *input, ttoken, '+');
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
			double_sized_token(input, *input, ttoken, 'h');
	}
	if ((**input) == '<')
		(*ttoken).type = t_r_in;
	if ((**input) == '>')
		(*ttoken).type = t_r_out;
	if ((**input) == '+')
		(*ttoken).type = t_r_outa;
	if ((**input) == 'h')
		(*ttoken).type = t_hd;
	if ((**input) == '|')
		(*ttoken).type = t_pipe;
}

int	get_token(char **input, t_vector *tokens)
{
	t_token	ttoken;

	while (**input && ft_isspace(**input))
		*input = *input + 1;
	if (!(**input))
		return (0);
	ttoken.start = *input;
	ttoken.size = 1;
	if (ft_strchr("<>|", **input))
	{
		parse_token(input, &ttoken);
		*input = *input + 1;
	}
	else
	{
		ttoken.type = t_word;
		while (**input && !(ft_strchr("<>|", **input)) && !ft_isspace(**input))
			*input = *input + 1;
		ttoken.size = (int)(*input - ttoken.start);
	}
	if (ttoken.size)
		ft_push_back(tokens, &ttoken);
	return (**input);
}

void	lexer(t_environment *env)
{
	int		token;
	char	*input_line;

	input_line = env->input_line;
	token = get_token(&input_line, &env->tokens);
	while (token)
		token = get_token(&input_line, &env->tokens);
}
