/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:34 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 16:28:20 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_token(char **input, t_vector *tokens, t_token *ttoken)
{
	(*ttoken).size = 1;
	(*ttoken).start = *input;
	if (ft_strchr("<>|", **input))
	{
		parse_token_sym(input, ttoken);
		*input = *input + 1;
	}
	else
	{
		(*ttoken).size = 0;
		parse_token_word(input, ttoken);
	}
	if ((*ttoken).size)
		ft_push_back(tokens, ttoken);
}

int	get_token(char **input, t_vector *tokens)
{
	t_token	ttoken;
	int		isspace_here;

	isspace_here = 0;
	ttoken.size = 1;
	while (**input && ft_isspace(**input))
	{
		*input = *input + 1;
		isspace_here = 1;
	}
	if (isspace_here)
	{
		ttoken.start = *input - 1;
		ttoken.type = t_sep;
		ft_push_back(tokens, &ttoken);
		return (**input);
	}
	if (!(**input))
		return (0);
	parse_token(input, tokens, &ttoken);
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
