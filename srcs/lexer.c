#include "minishell.h"

int	preparse(t_environment *env)
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

int	get_token(char **input, t_vector *tokens)
{
	int res;
	t_token ttoken;

	while (**input && ft_isspace(**input))
		*input = *input + 1;
	if (!(**input))
		return (0);
	res = (unsigned char)**input;
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
		ttoken.type = t_word_exp;
		while (**input && !(ft_strchr("<>|", **input)) && !ft_isspace(**input))
			*input = *input + 1;
		ttoken.size = (int)(*input - ttoken.start);
		if (ttoken.size)
			ft_push_back(tokens, (void *)&ttoken);
    }
	return (res);
}

void	lexer(t_environment *env)
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
		printf("%s", ft_substr(cur_token->start, 0, cur_token->size));
	}
	printf("\n");
	// debug end
}

int	add_redirect(t_vector *redirs, t_token *token, t_vector *tokens, size_t *i)
{
	t_redir	redir;
	t_token	*next_token;

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

char	*str_slash(char *current_string, int *i)
{
	char	*result;
	char	*firstpart;
	char	*lastpart;

	firstpart = ft_substr(current_string, 0, *i);
	lastpart = ft_strdup(current_string + *i);
	result = ft_strjoin(firstpart, lastpart);
	return (result);
}

char	*str_qoutes(char *current_string, int *i)
{
	int		beginning;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	beginning = *i;
	while (current_string[++*i])
	{
		if (current_string[*i] == '\'')
			break ;
	}
	firstpart = ft_substr(current_string, 0, beginning);
	in_qoutes_part = ft_substr(current_string, beginning + 1, (*i - beginning - 1));
	lastpart = ft_strdup(current_string + *i + 1);
	result = ft_strjoin(firstpart, in_qoutes_part);
	result = ft_strjoin(result, lastpart);
	printf("quotes res:%s\n", result);
	return (result);
}

char *str_expanding(char *current_string, int *i, t_environment *env)
{
	int				beginning;
	char			*result;
	char			*firstpart;
	char			*var_name;
	size_t			var_index;
	char			*var_value;
	char			*lastpart;
	t_variable_env	*var_element;

	beginning = *i;
	while (current_string[++(*i)])
	{
		if (!ft_isalnum(current_string[*i]) && current_string[*i] != '_')
			break;
	}
	firstpart = ft_substr(current_string, 0, beginning);
	var_name = ft_substr(current_string, beginning + 1, *i - beginning - 1);
	var_index = ft_find_by_name(&env->variables_env, var_name);
	var_element = ft_get_element(&env->variables_env, var_index);
	if (var_element)
		var_value = var_element->value;
	else
		var_value = ft_strdup("");
	lastpart = ft_strdup(current_string + *i);
	if (ft_strlen(var_name) > ft_strlen(var_value))
		*i = *i - (int)(ft_strlen(var_name) - ft_strlen(var_value));
	result = ft_strjoin(firstpart, var_value);
	result = ft_strjoin(result, lastpart);
	return (result);
}

char	*str_double_qoutes(char *current_string, int *i, t_environment *env)
{
	int		beginning;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	beginning = *i;
	while (current_string[++(*i)])
	{
		if (current_string[*i] == '\\' && (current_string[*i + 1] == '\\' || current_string[*i + 1] == '$' || current_string[*i + 1] == '\"'))
			current_string = str_slash(current_string, i);
		if (current_string[*i] == '$')
			current_string = str_expanding(current_string, i, env);
		if (current_string[*i] == '\"')
			break ;
	}
	firstpart = ft_substr(current_string, 0, beginning);
	in_qoutes_part = ft_substr(current_string, beginning + 1, (*i - beginning - 1));
	lastpart = ft_strdup(current_string + *i + 1);
	result = ft_strjoin(firstpart, in_qoutes_part);
	result = ft_strjoin(result, lastpart);
	printf("double quotes res:%s\n", result);
	return (result);
}

void	expand_word(t_environment *env, char **start, int *size)
{
	char	*current_string;
	int		i;

	current_string = ft_substr(*start, 0, *size);
	i = -1;
	while (current_string[++i])
	{
		if (current_string[i] == '\'')
			current_string = str_qoutes(current_string, &i);
		if (current_string[i] == '\\')
			current_string = str_slash(current_string, &i);
		if (current_string[i] == '\"')
			current_string = str_double_qoutes(current_string, &i, env);
		if (current_string[i] == '$')
			current_string = str_expanding(current_string, &i, env);
	}
	*start = current_string;
	*size = (int)ft_strlen(current_string);
}

void	get_command(t_environment *env, size_t *i)
{
	t_command	cmd;
	t_token		*cur_token;
	int			com_added;

	ft_init_vector(&cmd.redirs, sizeof(t_redir));
	ft_init_vector(&cmd.args, sizeof(t_vector));
	com_added = 0;
	while (*i < ft_size(&env->tokens))
	{
		cur_token = (t_token *)ft_get_element(&env->tokens, *i);
		expand_word(env, &cur_token->start, &cur_token->size);
		if (cur_token->type == t_pipe)
			break;
		add_redirect(&cmd.redirs, cur_token, &env->tokens, i);
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
	ft_push_back(&env->groups, (void *)&cmd);
}

void	parser(t_environment *env)
{
	size_t		i;
	t_command	*cur_cmd;

	ft_init_vector(&env->groups, sizeof(t_command));
	i = -1;
	while (++i < ft_size(&env->tokens))
		get_command(env, &i);
	// debug start
	i = -1;
	printf("Команды: \n");
	while (++i < ft_size(&env->groups))
	{
		cur_cmd = (t_command *)ft_get_element(&env->groups, i);
		printf("┌команда──%s\n", ft_substr(cur_cmd->command->start, 0, cur_cmd->command->size));

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
}

void	free_paths(char	**paths)
{
	char	*path;
	char	**paths_bkp;

	paths_bkp = paths;
	while (*paths_bkp)
	{
		path = *paths_bkp;
		free(path);
		paths_bkp++;
	}
	free(paths);
}

char	**find_path(char **envp)
{
	char	**env_path;

	env_path = envp;
	while (*env_path)
	{
		if (ft_strncmp(*env_path, "PATH=", 5))
			env_path = env_path + 1;
		else
		{
			*env_path = *env_path + 5;
			break ;
		}
	}
	if (*env_path == NULL)
		return (NULL);
	return (ft_split(*env_path, ':'));
}

void	ft_full_path(char **in_ar, char **paths, char **path, char **full_path)
{
	(*path) = ft_strjoin(*paths, "/");
	if (!(*path))
		exit(EXIT_FAILURE);
	(*full_path) = ft_strjoin((*path), in_ar[0]);
	if (!(*full_path))
	{
		free((*path));
		exit(EXIT_FAILURE);
	}
}

void	find_cmd_in_path(char **args, char **envp)
{
	char	**paths;
	char	*path;
	char	*full_path;
	char	*access_denied_path;
	char	**paths_bkp;

	paths = find_path(envp);
	if (!paths)
		cmd_not_found(args);
	access_denied_path = NULL;
	paths_bkp = paths;
	while (*paths)
	{
		ft_full_path(args, paths, &path, &full_path);
		execve(full_path, args, envp);
		if (errno == EACCES)
			access_denied_path = ft_strdup(full_path);
		free(full_path);
		free(path);
		paths++;
	}
	free_paths(paths_bkp);
	exit_find_failure(args, access_denied_path);
}

void	ft_exec_command(t_environment *env, t_command *cmd)
{
	char 	*command;
	char 	**envp;
	char 	**args;

	command = ft_substr(cmd->command->start, 0, cmd->command->size);
	if (ft_strlen(command) == 0)
		exit(0);
	ft_convert_vector_to_array(&envp, &env->variables_env);
	ft_convert_token_vector_to_str_array(&args, &cmd->args);
	if (ft_strchr(command, '/') != NULL)
		execve(command, args, envp);
	find_cmd_in_path(args, envp);
}

void	child_process(t_environment *env, size_t i, int pipe_fd[2][2])
{
	t_command	*cur_cmd;

	cur_cmd = (t_command *) ft_get_element(&env->groups, i);
	size_t r=-1;
	t_redir *cur_redir;
	if (i != 0)
	{
		if (dup2(pipe_fd[!(i % 2)][0], 0) == -1)
			ft_raise_error("dup2 error 0\n");
	}
	if ((i + 1 != ft_size(&env->groups)))
	{
		if (dup2(pipe_fd[i % 2][1], 1) == -1)
			ft_raise_error("dup2 error 1\n");
	}
	while (++r < ft_size(&cur_cmd->redirs))
	{
		cur_redir = (t_redir *)ft_get_element(&cur_cmd->redirs, r);
		if (cur_redir->r_type == t_r_in)
			input_file_fd(cur_redir);
		if (cur_redir->r_type == t_hd)
			here_doc(cur_redir, pipe_fd[i % 2]);
		if (cur_redir->r_type == t_r_out || cur_redir->r_type == t_r_outa)
			output_file_fd(cur_redir);
	}
	ft_exec_command(env, cur_cmd);
}

int	executor(t_environment *env)
{
	size_t	current;
	pid_t	pid;
	int		pipe_fd[2][2];

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		pipe(pipe_fd[current % 2]);
		pid = fork();
		if (pid == -1)
			ft_raise_error("fork error\n");
		else if (pid == 0)
			child_process(env, current, pipe_fd);
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
