#include "minishell.h"

void	ft_exec_command(t_environment *env, t_cmd *cmd, int is_child)
{
	char	*command;
	char	**envp;
	char	**args;

	ft_token_vector_to_str_array(&args, &cmd->args);
	if (cmd->builtin)
	{
		if (is_child)
			exit(cmd->builtin->func(env, (const char *const *)(args + 1)));
		cmd->status = cmd->builtin->func(env, (const char *const *)(args + 1));
		free_command_args(args);
	}
	else
	{
		command = ft_substr(cmd->command->start, 0, cmd->command->size);
		ft_convert_vector_to_array(&envp, &env->variables_env);
		if (ft_strchr(command, '/') != NULL)
			execve(command, args, envp);
		find_cmd_in_path(args, envp);
	}
}