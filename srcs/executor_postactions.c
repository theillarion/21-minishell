/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_postactions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:09:34 by illarion          #+#    #+#             */
/*   Updated: 2022/09/29 15:09:36 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(const t_environment *env)
{
	size_t	i;
	t_token	*token;

	i = 0;
	while (i < ft_size(&env->tokens))
	{
		token = ft_get_element(&env->tokens, i);
		free(token->start);
		if (token->type == t_r_out || token->type == t_r_outa
			|| token->type == t_r_in || token->type == t_hd)
		{
			++i;
			token = ft_get_element(&env->tokens, i);
			if (token->type == t_sep)
				i++;
		}
		else
			i++;
	}
}

void	free_command_args(char **args)
{
	char	**args_bkp;

	args_bkp = args;
	while (*args_bkp)
	{
		free(*args_bkp);
		args_bkp++;
	}
	free(args);
}

void	ft_free_cmd_vectors(t_environment *env)
{
	size_t	i;
	t_cmd	*current;

	i = -1;
	while (++i < ft_size(&env->groups))
	{
		current = ft_get_element(&env->groups, i);
		ft_erase_all(&current->args);
		ft_erase_all(&current->redirs);
	}
	ft_erase_all(&env->groups);
	free_tokens(env);
	ft_erase_all(&env->tokens);
}

void	postactions(t_environment *env)
{
	ft_free_cmd_vectors(env);
}
