/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expanding.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:47 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 16:31:27 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_slash(char *cs, int *i)
{
	char	*result;
	char	*firstpart;
	char	*lastpart;

	firstpart = ft_substr(cs, 0, *i);
	lastpart = ft_strdup(cs + *i);
	result = ft_strjoin_with_free(firstpart, lastpart, 1, 1);
	(*i) = (*i) + 2;
	free(cs);
	return (result);
}

char	*str_qoutes(char *cs, int *i, int *r)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	*r = 1;
	while (cs[++(*i)])
	{
		if (cs[*i] == '\'')
		{
			*r = 0;
			break ;
		}
	}
	firstpart = ft_substr(cs, 0, begin);
	in_qoutes_part = ft_substr(cs, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(cs + *i + 1);
	*i = ft_strlen(firstpart) + ft_strlen(in_qoutes_part) - 1;
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(cs);
	return (result);
}

char	*str_expanding(char *cs, int *i, t_environment *env)
{
	int				beginning;
	char			*result;
	char			*firstpart;
	char			*lastpart;
	char			*var_value;

	beginning = *i;
	while (cs[++(*i)])
	{
		if (chk_end_exp_w(cs, i, beginning))
			break ;
	}
	firstpart = ft_substr(cs, 0, beginning);
	lastpart = ft_strdup(cs + *i);
	if (cs[(*i) - 1] == '?')
		var_value = ft_itoa(env->last_code);
	else
		var_value = get_v(cs, i, env, beginning);
	if (var_value)
		*i = ft_strlen(firstpart) + ft_strlen(var_value) - 1;
	else
		*i = ft_strlen(firstpart) - 1;
	get_result_str(&result, firstpart, lastpart, var_value);
	free(cs);
	return (result);
}

char	*str_doub_qoutes(char *cs, int *i, t_environment *env, int *r)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	*r = 2;
	go_throw_string(&cs, i, env, r);
	firstpart = ft_substr(cs, 0, begin);
	in_qoutes_part = ft_substr(cs, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(cs + *i + 1);
	*i = ft_strlen(firstpart) + ft_strlen(in_qoutes_part) - 1;
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(cs);
	return (result);
}

int	expand_word(t_environment *env, char **start, int *size)
{
	char	*current_string;
	int		i;
	int		err;

	current_string = ft_substr(*start, 0, *size);
	if (!current_string)
		return (1);
	i = 0;
	err = 0;
	while (current_string && current_string[i])
	{
		if (parse_symbols(env, &current_string, &i, &err) == 0)
			break ;
	}
	*start = current_string;
	if (current_string)
		*size = (int)ft_strlen(current_string);
	else
		*size = 0;
	if (err)
		return (ft_syntax_error(env));
	return (err);
}
