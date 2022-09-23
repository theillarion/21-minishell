/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 22:53:06 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 22:53:06 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_destroy(t_environment	*env)
{
	ft_clear_vector(&env->variables_env);
	ft_clear_vector(&env->functions);
	ft_smart_free((void **)&env->prompt.current_prompt);
	ft_smart_free((void **)&env->info.pwd);
	ft_smart_free((void **)&env->info.name_shell);
	ft_smart_free((void **)&env->input_line);
	ft_smart_double_free((void ***)&env->envp);
}
