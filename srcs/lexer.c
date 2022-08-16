#include "../includes/minishell.h"
#include "../includes/ft_token.h"

enum e_Token
{
	t_word,
	t_word_exp,
	t_pipe,
	t_r_out,
	t_r_in,
	t_hd,
	t_r_outa
};

enum e_RedirType
{
	r_in,
	r_out,
	r_out_append,
	r_here_doc
};

typedef struct s_token
{
	char	*start;
	int		size;
	int		type;
}	t_token;

typedef struct s_redir
{
	int r_type;
	char *arg;
}	t_redir;

typedef struct s_command
{
	char *command;
	char *args;
	t_vector redirs;
}	t_command;

int preparse(char *input)
{
	// ToDo Write checks for bash syntax
	while (*input)
	{
		input++;
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

void lexer(char *input, t_vector *tokens)
{
	int token;
	size_t 	i;
	t_token *cur_token;

	token = get_token(&input, tokens);
	printf("Типы токенов: "); // debug
	while(token)
	{
		printf("%c", token); // debug
		token = get_token(&input, tokens);
	}
	// debug start
	printf("\nКоличество токенов в векторе: ");
	printf("ft_size: %zu\n", ft_size(tokens));
	i = -1;
	printf("Вывод значений токенов: ");
	while (++i < ft_size(tokens))
	{
		cur_token = (t_token *)ft_get_element(tokens, i);
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
	redir.arg = ft_substr(next_token->start, 0, next_token->size);
	if (token->type == t_hd)
		redir.r_type = r_here_doc;
	if (token->type == t_r_in)
		redir.r_type = r_in;
	if (token->type == t_r_out)
		redir.r_type = r_out;
	if (token->type == t_r_outa)
		redir.r_type = r_out_append;
	ft_push_back(redirs, (void *)&redir);
	return (1);
}

void get_command(t_vector *groups, t_vector *tokens, size_t *i)
{
	t_command cmd;
	t_token *cur_token;
	int com_added;

	ft_init_vector(&cmd.redirs, sizeof(t_redir));
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
				cmd.command = ft_substr(cur_token->start, 0, cur_token->size);
				cmd.args = ft_substr(cur_token->start, 0, cur_token->size);
			}
			else
			{
				cmd.args = ft_strjoin(cmd.args, " ");
				cmd.args = ft_strjoin(cmd.args, ft_substr(cur_token->start, 0, cur_token->size));
			}
		}
		(*i)++;
	}
	ft_push_back(groups, (void *)&cmd);
}

void parser(t_vector *tokens)
{
	size_t i;
	t_vector groups;
	t_command *cur_cmd;

	ft_init_vector(&groups, sizeof(t_command));
	i = -1;
	while (++i < ft_size(tokens))
		get_command(&groups, tokens, &i);
	// debug start
	i = -1;
	printf("Команды: \n");
	while (++i < ft_size(&groups))
	{
		cur_cmd = (t_command *)ft_get_element(&groups, i);
		printf("┌команда───%s\n", cur_cmd->command);
		if (ft_size(&cur_cmd->redirs))
		{
			printf("├аргументы──%s\n", cur_cmd->args);
			printf("└редиректы");
			size_t r=-1;
			t_redir *cur_redir;
			while (++r < ft_size(&cur_cmd->redirs))
			{
				cur_redir = (t_redir *)ft_get_element(&cur_cmd->redirs, r);
				printf("─%d─%s──", cur_redir->r_type, cur_redir->arg);
			}
			printf("\n");
		}
		else
			printf("└аргументы──%s\n", cur_cmd->args);
	}
	printf("\n");
	// debug end
}

int	main(int argc, char **argv, char **envp)
{
//	t_vector	*tokens;
	(void)		argc;
	(void)		envp;
	(void)		argv;
	char*		in;
	char		prompt[12] = "minishell: ";
	t_vector	tokens;

	in = readline(prompt);
	while (in)
	{
		if (preparse(in))
		{
			ft_init_vector(&tokens, sizeof(t_token));
			lexer(in, &tokens);
			parser(&tokens);
		}
		add_history(in);
		in = readline(prompt);
	}
}
