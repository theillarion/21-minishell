#include "../includes/minishell.h"

int preparse(t_environment *env)
{
	char *il;

	// ToDo Write checks for bash syntax
	il = env->input_line;
	while (*il)
	{
		il++;
	}
	return (1);
}

int get_token(char **input, t_vector *tokens)
{
	int res;
	t_token ttoken;

	//ft_reserve(tokens, tokens->capacity + 1);
	while (**input && ft_isspace(**input))
		*input = *input + 1;
	if (!(**input))
		return (0);
	res = **input;
    if (ft_strchr("<>|", res))
    {
		ttoken.start = *input;
		ttoken.size = 1;
        if (**input == '>')
        {
            if (*(*input + 1) == '>')
			{
				res = '+';
				ttoken.size = 2;
				*input = *input + 1;
			}
        }
        else if (**input == '<')
        {
            if (*(*input + 1) == '<')
			{
				res = 'h';
				ttoken.size = 2;
				*input = *input + 1;
			}
        }
		if (res == '<')
			ttoken.type = t_r_in;
		if (res == '>')
			ttoken.type = t_r_out;
		if (res == '+')
			ttoken.type = t_r_outa;
		if (res == 'h')
			ttoken.type = t_hd;
		if (res == '|')
			ttoken.type = t_pipe;
		ft_push_back(tokens, &ttoken);
        *input = *input + 1;
    }
	else
    {
        res = 'w';
		ttoken.start = *input;
		ttoken.type = t_word;
        while (**input && !(ft_strchr("<>|", **input)) && !ft_isspace(**input))
		{
			if (ft_strchr("\'", **input))
			{
				*input = *input + 1;
				while (**input && !ft_strchr("\'", **input))
					*input = *input + 1;
			}
			if (ft_strchr("\"", **input))
			{
				res = 'e';
				*input = *input + 1;
				while (**input && !ft_strchr("\"", **input))
					*input = *input + 1;
				ttoken.type = t_word_exp;
			}
			*input = *input + 1;
		}
		ttoken.size = *input - ttoken.start;
		ft_push_back(tokens, (void *)&ttoken);
    }
	//*input = *input + 1;
	return (res);
}

void lexer(t_environment *env)
{
	int token;
	size_t 	i;
	t_token *cur_token;
	char *il;

	il = env->input_line;
	token = get_token(&il, &env->tokens);
	printf("Типы токенов: "); // debug
	while(token)
	{
		printf("%c", token); // debug
		token = get_token(&il, &env->tokens);
	}
	// debug start
	printf("\nКоличество токенов в векторе: ");
	printf("ft_size: %zu\n", ft_size(&env->tokens));
	i = -1;
	printf("Вывод значений токенов: ");
	while (++i < ft_size(&env->tokens))
	{
		cur_token = (t_token *)ft_get_element(&env->tokens, i);
		printf("%s ", ft_substr(cur_token->start, 0, cur_token->size));
	}
	printf("\n");
	// debug end
}

int add_redirect(t_vector *redirs, t_token *token, t_vector *tokens, size_t *i)
{
	t_redir redir;
	t_token *next_token;

	if (token->type != t_r_in && token->type != t_r_out && token->type != t_r_outa && token->type != t_hd)
		return (0);
	(*i)++;
	next_token = (t_token *)ft_get_element(tokens, *i);
	if (!next_token)
		return (0);
	redir.arg = next_token;
	redir.r_type = token->type;
	ft_push_back(redirs, (void *)&redir);
	return (1);
}

void get_command(t_vector *groups, t_vector *tokens, size_t *i)
{
	t_command cmd;
	t_token *cur_token;
	int com_added;

	ft_init_vector(&cmd.redirs, sizeof(t_redir));
	ft_init_vector(&cmd.args, sizeof(t_vector));
	com_added = 0;
	while (*i < ft_size(tokens))
	{
		cur_token = (t_token *)ft_get_element(tokens, *i);
		if (cur_token->type == t_pipe)
			break;
		add_redirect(&cmd.redirs, cur_token, tokens, i);
		if (cur_token->type == t_word || cur_token->type == t_word_exp)
		{
			if (!com_added)
			{
				com_added = 1;
				cmd.command = cur_token;
			}
			ft_push_back(&cmd.args, (void *)cur_token);
		}
		(*i)++;
	}
	ft_push_back(groups, (void *)&cmd);
}

void parser(t_environment *env)
{
	size_t i;
	t_command *cur_cmd;

	ft_init_vector(&env->groups, sizeof(t_command));
	i = -1;
	while (++i < ft_size(&env->tokens))
		get_command(&env->groups, &env->tokens, &i);
	// debug start
	i = -1;
	printf("Команды: \n");
	while (++i < ft_size(&env->groups))
	{
		cur_cmd = (t_command *)ft_get_element(&env->groups, i);
		printf("┌команда───%s\n", ft_substr(cur_cmd->command->start, 0, cur_cmd->command->size));

		if (ft_size(&cur_cmd->redirs))
		{
			printf("├аргументы──");
			size_t j = -1;
			t_token *cur_arg;
			while (++j < ft_size(&cur_cmd->args))
			{
				cur_arg = (t_token *)ft_get_element(&cur_cmd->args, j);
				printf("%d─%s──", cur_arg->type, ft_substr(cur_arg->start, 0, cur_arg->size));
			}
			printf("\n");

			printf("└редиректы──");
			size_t r=-1;
			t_redir *cur_redir;
			while (++r < ft_size(&cur_cmd->redirs))
			{
				cur_redir = (t_redir *)ft_get_element(&cur_cmd->redirs, r);
				printf("%d─%s──", cur_redir->r_type, ft_substr(cur_redir->arg->start, 0, cur_redir->arg->size));
			}
			printf("\n");
		}
		else
		{
			printf("└аргументы──");
			size_t j = -1;
			t_token *cur_arg;
			while (++j < ft_size(&cur_cmd->args))
			{
				cur_arg = (t_token *)ft_get_element(&cur_cmd->args, j);
				printf("%d─%s──", cur_arg->type, ft_substr(cur_arg->start, 0, cur_arg->size));
			}
			printf("\n");
		}
	}
	printf("\n");
	// debug end
	i = -1;
	while (++i < ft_size(&env->groups))
	{
		cur_cmd = (t_command *) ft_get_element(&env->groups, i);
	}
}

void expand_vars(t_environment *env, int current)
{

}

void	ft_exec_command(t_environment *env, t_command *cmd)
{
	char	**in_argv;
	char 	*command;
	char 	**envp;

	command = ft_substr(cmd->command->start, 0, cmd->command->size);
	envp = get_envp_array(env);
	if (ft_strchr(command, '/') != NULL)
		execve(command, , pipex.envp);
	find_cmd_in_path(pipex, in_argv);
}

void	child_process(t_environment *env, int i, int pipe_fd[2][2])
{
	t_command *cur_cmd;

	cur_cmd = (t_command *) ft_get_element(&env->groups, i);

	//printf("┌команда───%s\n", ft_substr(cur_cmd->command->start, 0, cur_cmd->command->size));

	//printf("├аргументы──");
	size_t j = -1;
	t_token *cur_arg;
	while (++j < ft_size(&cur_cmd->args))
	{
		cur_arg = (t_token *)ft_get_element(&cur_cmd->args, j);
		//printf("%d─%s──", cur_arg->type, ft_substr(cur_arg->start, 0, cur_arg->size));
	}
	//printf("\n");

	//printf("└редиректы──");
	size_t r=-1;
	t_redir *cur_redir;
	if (dup2(pipe_fd[! (i % 2)][0], 0) == -1)
		ft_raise_error("dup2 error\n");
	if (dup2(pipe_fd[i % 2][1], 1) == -1)
		ft_raise_error("dup2 error\n");
	while (++r < ft_size(&cur_cmd->redirs))
	{
		cur_redir = (t_redir *)ft_get_element(&cur_cmd->redirs, r);
		if (cur_redir->r_type == t_r_in)
			input_file_fd(cur_redir);
		if (cur_redir->r_type == t_hd)
			here_doc(cur_redir, pipe_fd[i % 2]);
		if (cur_redir->r_type == t_r_out || cur_redir->r_type == t_r_outa)
			output_file_fd(cur_redir);
		//printf("%d─%s──", cur_redir->r_type, ft_substr(cur_redir->arg->start, 0, cur_redir->arg->size));
	}


//	if (i == 0)
//	{
//		if (!pipex->is_heredoc)
//			input_file_fd(pipex);
//		else
//			here_doc(argv, pipe_fd[i % 2]);
//	}
////	else
//	if (dup2(pipe_fd[! (i % 2)][0], 0) == -1)
//		ft_raise_error("dup2 error\n");
//	if (i == pipex->cmd_cnt - 1)
//		output_file_fd(pipex);
//	else
//	if (dup2(pipe_fd[i % 2][1], 1) == -1)
//		ft_raise_error("dup2 error\n");
	ft_exec_command(cur_cmd);
}

int executor(t_environment *env)
{
	size_t	current;
	pid_t	pid;
	int		pipe_fd[2][2];

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		expand_vars(&env, current);
		pipe(pipe_fd[current % 2]);
		pid = fork();
		if (pid == -1)
			ft_raise_error("fork error\n");
		else if (pid == 0)
			child_process(&env, current, pipe_fd);
		else
		if (close(pipe_fd[current % 2][1]) == -1)
			ft_raise_error("close fd error\n");
	}
	return (pid);
}

//int	main(int argc, char **argv, char **envp)
//{
////	t_vector	*tokens;
//	(void)		argc;
//	(void)		envp;
//	(void)		argv;
//	char*		in;
//	char		prompt[12] = "minishell: ";
//	t_vector	tokens;
//
//	in = readline(prompt);
//	while (in)
//	{
//		if (preparse(in))
//		{
//			ft_init_vector(&tokens, sizeof(t_token));
//			lexer(in, &tokens);
//			parser(&tokens);
//		}
//		add_history(in);
//		in = readline(prompt);
//	}
//}
