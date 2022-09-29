/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expanding_utilities.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltowelie <ltowelie@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:43 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 15:59:50 by ltowelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_symbols(t_environment *env, char **cs, int *i, int *err)
{
	if ((*cs)[(*i)] == '\'')
	{
		(*cs) = str_qoutes((*cs), i, err);
		if ((*err))
			return (0);
	}
	if ((*cs)[(*i)] == '\"')
	{
		(*cs) = str_doub_qoutes((*cs), i, env, err);
		if ((*err))
			return (0);
	}
	if ((*cs)[(*i)] == '\\')
		(*cs) = str_slash((*cs), i);
	if ((*cs)[(*i)] == '$')
		(*cs) = str_expanding((*cs), i, env);
	else
		(*i)++;
	return (1);
}

void	go_throw_string(char **cs, int *i, t_environment *env, int *r)
{
	while ((*cs)[++(*i)])
	{
		if ((*cs)[*i] == '\\' && ((*cs)[*i + 1] == '\\'
				|| (*cs)[*i + 1] == '$' || (*cs)[*i + 1] == '\"'))
			(*cs) = str_slash((*cs), i);
		if ((*cs)[*i] == '\"')
		{
			*r = 0;
			break ;
		}
		if ((*cs)[*i] == '$')
			(*cs) = str_expanding((*cs), i, env);
	}
}

void	get_result_str(char **r, char *fp, char *lp, char *val)
{
	if (!val && !ft_strlen(fp) && !ft_strlen(lp))
	{
		free(fp);
		free(lp);
		*r = ft_strdup("");
	}
	else
	{
		*r = ft_strjoin_with_free(fp, val, 1, 1);
		*r = ft_strjoin_with_free(*r, lp, 1, 1);
	}
}

int	chk_end_exp_w(const char *cs, int *i, int beginning)
{
	if (*i == beginning + 1 && cs[*i] == '?' && (*i)++)
		return (1);
	if (*i == beginning + 1 && !ft_isalpha(cs[*i]) && (*i)++)
		return (1);
	if (!ft_isalnum(cs[*i]) && cs[*i] != '_')
		return (1);
	return (0);
}
