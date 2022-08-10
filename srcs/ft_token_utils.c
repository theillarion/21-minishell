/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltowelie <ltowelie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 19:19:11 by ltowelie          #+#    #+#             */
/*   Updated: 2022/03/05 14:39:47 by ltowelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/ft_token.h"

int	dull_handler(char *p, char **st_of_w)
{
	int		c;
	int		state;

	state = 0;
	c = (unsigned char)*p;
	if (ft_isspace(c))
		return (state);
	if (c == '"')
	{
		state = 2;
		(*st_of_w) = p + 1;
		return (state);
	}
	if (c == '\'')
	{
		state = 3;
		(*st_of_w) = p + 1;
		return (state);
	}
	state = 1;
	*st_of_w = p;
	return (state);
}

int	inword_handler(t_vector **argv, char *p, char *st_of_w)
{
	int		c;
	int		state;

	state = 1;
	c = (unsigned char)*p;
	if (ft_isspace(c))
	{
		*p = 0;
		ft_push_back(*argv, &st_of_w);
		state = 0;
	}
	return (state);
}

int	instring_dq_handler(t_vector **argv, char *p, char *st_of_w)
{
	int		c;
	int		state;

	state = 2;
	c = (unsigned char)*p;
	if (c == '"')
	{
		*p = 0;
		ft_push_back(*argv, &st_of_w);
		state = 0;
	}
	return (state);
}

int	instring_q_handler(t_vector **argv, char *p, char *st_of_w)
{
	int		c;
	int		state;

	state = 3;
	c = (unsigned char)*p;
	if (c == '\'')
	{
		*p = 0;
		ft_push_back(*argv, &st_of_w);
		state = 0;
	}
	return (state);
}
