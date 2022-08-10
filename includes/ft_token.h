/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltowelie <ltowelie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 19:27:48 by ltowelie          #+#    #+#             */
/*   Updated: 2022/03/05 14:05:11 by ltowelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TOKEN_H
# define FT_TOKEN_H

# include <stdlib.h>

enum	e_States
{
	DULL,
	INWORD,
	INSTRING_DQ,
	INSTRING_Q,
};

int		ft_isspace(int c);

int		dull_handler(char *p, char **st_of_w);
int		inword_handler(t_vector **argv, char *p, char *st_of_w);
int		instring_dq_handler(t_vector **argv, char *p, char *st_of_w);
int		instring_q_handler(t_vector **argv, char *p, char *st_of_w);

void	ft_split_with_tokens(const char *s, t_vector	*v_argv);

#endif