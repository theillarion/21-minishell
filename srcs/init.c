/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 23:00:15 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:00:31 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_delete_element(void	*address)
{
	t_variable_env	*var_env;

	var_env = (t_variable_env *)address;
	ft_smart_free((void **)&var_env->name);
	ft_smart_double_free((void ***)&var_env->values);
}

void	ft_init(t_environment	*env)
{
	if (env == NULL)
		return ;
	ft_init_vector(&env->variables_env, sizeof(t_variable_env),
		&ft_delete_element);
	ft_init_vector(&env->functions, sizeof(t_function), NULL);
	env->info.name_shell = NULL;
	env->info.pwd = NULL;
	env->prompt.current_prompt = NULL;
	env->prompt.is_need_change = false;
	env->input_line = NULL;
	env->envp = NULL;
	env->is_need_update_envp = false;
	env->last_code = 0;
}
