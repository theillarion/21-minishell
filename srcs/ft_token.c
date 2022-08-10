/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltowelie <ltowelie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 19:19:11 by ltowelie          #+#    #+#             */
/*   Updated: 2022/03/05 14:38:55 by ltowelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/ft_token.h"

void	st_handlers(t_vector **argv, char *p, char **st_of_w,
			int *state)
{
	if (*state == DULL)
	{
		*state = dull_handler(p, st_of_w);
		return ;
	}
	if (*state == INSTRING_DQ)
	{
		*state = instring_dq_handler(argv, p, *st_of_w);
		return ;
	}
	if (*state == INSTRING_Q)
	{
		*state = instring_q_handler(argv, p, *st_of_w);
		return ;
	}
	if (*state == INWORD)
	{
		*state = inword_handler(argv, p, *st_of_w);
		return ;
	}
}

size_t	ft_split_token(char *buffer, t_vector *argv)
{
	char	*p;
	char	*st_of_w;
	int		state;

	state = 0;
	p = buffer;
	while (*p != '\0')
	{
		st_handlers(&argv, p, &st_of_w, &state);
		p++;
	}
	if (state != 0)
	{
		ft_push_back(argv, &st_of_w);
	}
	return (0);
}

void	ft_split_with_tokens(const char *s, t_vector *v_argv)
{
//	size_t	v_size;
//	t_vector	v_argv;
//	char	**argv;
//	int		i;
//	char **current_string;

	ft_init_vector(v_argv, sizeof(char *));
	ft_split_token((char *)s, v_argv);
//	v_size = ft_size(v_argv);
//	argv = ft_calloc((v_size + 1), sizeof(char *));
//	if (!argv)
//		exit(EXIT_FAILURE);
//	i = -1;
//	while ((size_t)++i < ft_size(&v_argv))
//	{
//		current_string = ft_get_element(&v_argv,  i);
//		argv[i] = ft_strdup(*current_string);
//	}
//	argv[v_size] = NULL;
//	ft_erase_all(&v_argv);
//	return (&v_argv);
}
