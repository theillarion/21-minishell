/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltowelie <ltowelie@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:29 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 15:59:54 by ltowelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void	double_sized_token(char **input, char *res, t_token *ttoken, char sym)
{
	(*res) = sym;
	(*ttoken).size = 1;
	if ((**input) == '+')
		(*ttoken).type = t_r_outa;
	if ((**input) == 'h')
		(*ttoken).type = t_hd;
	*input = *input + 1;
}

void	parse_token_sym(char **input, t_token *ttoken)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
			double_sized_token(input, *input, ttoken, '+');
		else
			(*ttoken).type = t_r_out;
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
			double_sized_token(input, *input, ttoken, 'h');
		else
			(*ttoken).type = t_r_in;
	}
	if ((**input) == '|')
		(*ttoken).type = t_pipe;
}

void	parse_token_word(char **input, t_token *ttoken)
{
	(*ttoken).type = t_word;
	while (**input && !(ft_strchr("<>|", **input)) && !ft_isspace(**input))
	{
		if (ft_strchr("\'", **input))
		{
			*input = *input + 1;
			while (**input && !ft_strchr("\'", **input))
				*input = *input + 1;
			*input = *input + 1;
			break ;
		}
		if (ft_strchr("\"", **input))
		{
			*input = *input + 1;
			while (**input && !ft_strchr("\"", **input))
				*input = *input + 1;
			*input = *input + 1;
			break ;
		}
		*input = *input + 1;
	}
	(*ttoken).size = (int)(*input - (*ttoken).start);
}
