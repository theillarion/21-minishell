#include "../includes/minishell.h"
#include "../includes/ft_token.h"

enum e_Token
{
	tok_word,
//	tok_eof,
//	tok_eol,
	tok_pipe,
	tok_gt,
	tok_lt,
	tok_dgt,
	tok_dlt,
//	tok_q,
//	tok_dq,
};
//
//typedef struct s_command
//{
//    char                *in;
//	int 				is_here_doc;
//	int 				input_closed;
//	int 				output_closed;
//	char				*out;
//	int 				append_mode;
//    char                *command;
//    int                 builtin;
//	t_vector 			*args;
//}   t_command;

//typedef struct s_token
//{
//	enum e_Token	tok_type;
//	int 			size;
//}	t_token;

//void	ft_raise_error(char *strarg)
//{
//	ft_putstr_fd(strarg, 2);
//}
//
//char	*int_add_char_to_string(char *s, char c)
//{
//	char *new;
//	char *res;
//
//	new = malloc(sizeof(char) * 2);
//	new[0] = c;
//	new[1] = '\0';
//	if (s != NULL)
//	{
//		res = ft_strjoin(s, new);
//		free(s);
//		free(new);
//	}
//	else
//		res = new;
//	return (res);
//}

//int parse_arg(char *arg)
//{
//	while (*arg)
//	{
//		if (*arg == '$')
//		{
//			char *var;
//
//			arg++;
//			var = NULL;
//			while (*arg && *arg != ' ' && *arg != '"' && *arg != '\'' && *arg != '$')
//			{
//				var = int_add_char_to_string(var, *arg);
//				arg++;
//			}
//		}
//		else
//			arg++;
//	}
//	return (0);
//}

//void check_new_command(int *new, t_command *com, t_vector **commands)
//{
//	if (*new)
//	{
//		*new = 0;
//		com = malloc(sizeof(t_command));
//		com->output_closed = 0;
//		com->input_closed = 0;
//		ft_tdlstadd_back(commands, ft_tdlstnew(com));
//	}
//}

//int parser(char **tokens)
//{
//	t_vector		**commands;
//	t_vector		*current_com;
//	t_command	*com;
//	int			new = 1;
//
//	commands = malloc(sizeof(t_vector **));
//	*commands = NULL;
//	while (*tokens)
//	{
//		// ToDo Add other token types to parsing
//		if (!ft_strncmp(*tokens, "<<", ft_strlen(*tokens)))
//		{
//			check_new_command(&new, com, commands);
//			current_com = ft_back(*commands);
//			if (((t_command *)current_com->c)->input_closed)
//				ft_raise_error("syntax error\n");
//			else
//			{
//				((t_command *) current_com->c)->is_here_doc = 1;
//				((t_command *) current_com->c)->input_closed = 1;
//			}
//		}
//		else if (!ft_strncmp(*tokens, "<", ft_strlen(*tokens)))
//		{
//			check_new_command(&new, com, commands);
//			current_com = ft_back(*commands);
//			if (((t_command *)current_com->c)->input_closed)
//				ft_raise_error("syntax error\n");
//			else
//			{
//				((t_command *) current_com->c)->input_closed = 1;
//			}
//		}
//		else if (!ft_strncmp(*tokens, ">>", ft_strlen(">>")))
//		{
//			check_new_command(&new, com, commands);
//			current_com = ft_back(*commands);
//			if (((t_command *)current_com->c)->output_closed)
//				ft_raise_error("syntax error\n");
//			else
//			{
//				((t_command *) current_com->c)->output_closed = 1;
//			}
//		}
//		else if (!ft_strncmp(*tokens, ">", ft_strlen(*tokens)))
//		{
//			check_new_command(&new, com, commands);
//			current_com = ft_back(*commands);
//			if (((t_command *)current_com->c)->output_closed)
//				ft_raise_error("syntax error\n");
//			else
//			{
//				((t_command *) current_com->c)->output_closed = 1;
//				((t_command *) current_com->c)->append_mode = 1;
//			}
//		}
//		else if (!ft_strncmp(*tokens, "|", ft_strlen(*tokens)))
//		{
//			new = 1;
//		}
//		else
//		{
//			parse_arg(*tokens);
//		}
//        tokens++;
//	}
//    return (0);
//}

int *lexer(t_vector *v_argv)
{
	int *lexems;
	char *token;
	size_t i;

	lexems = malloc(ft_size(v_argv));
	i = -1;
	while (++i < ft_size(v_argv))
	{
		token = *((char **)ft_get_element(v_argv, i));
		if (!ft_strncmp(token, "<<", ft_strlen("<<")))
		{
			lexems[i] = tok_dlt;
		}
		else if (!ft_strncmp(token, "<", ft_strlen("<")))
		{
			lexems[i] = tok_lt;
		}
		else if (!ft_strncmp(token, ">>", ft_strlen(">>")))
		{
			lexems[i] = tok_dgt;
		}
		else if (!ft_strncmp(token, ">", ft_strlen(">")))
		{
			lexems[i] = tok_gt;
		}
		else if (!ft_strncmp(token, "|", ft_strlen("|")))
		{
			lexems[i] = tok_pipe;
		}
		else
		{
			lexems[i] = tok_word;
		}
	}
	return lexems;
}

int	main(int argc, char **argv, char **envp)
{
//	t_vector	*tokens;
    (void) argc;
    (void) envp;
    (void) argv;
    char*   in;
    char   prompt[12] = "minishell: ";
	t_vector	v_argv;
	int			*lexems;
	int i;
//	int el;
    in = readline(prompt);
	while (in)
	{
		ft_split_with_tokens((const char *)in, &v_argv);
		i = -1;
		while ((size_t)++i < ft_size(&v_argv))
		{
			printf("%s\n", *((char **)ft_get_element(&v_argv, i)));
		}
		//exit(0);
		lexems = lexer(&v_argv);
		i = -1;
		while ((size_t)++i < ft_size(&v_argv))
		{
			printf("%d ", lexems[i]);
		}
		printf("\n");
        //parser(tokens);

//        while (*tokens)
//        {el
//            printf("%s\n", *tokens);
//			tokens++;
//        }
//		add_history(in);
//		in = readline(prompt);
		in = NULL;
	}
}