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
		ttoken.type = t_word;
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
	char *input_line;

	input_line = env->input_line;
	token = get_token(&input_line, &env->tokens);
	while(token)
		token = get_token(&input_line, &env->tokens);
	// debug start
//// printf("Типы токенов: "); // debug
////	printf("%c", token); // debug
//	size_t 	i;
//	t_token *cur_token;
//	printf("\nКоличество токенов в векторе: ");
//	printf("ft_size: %zu\n", ft_size(&env->tokens));
//	i = -1;
//	printf("Вывод значений токенов: ");
//	while (++i < ft_size(&env->tokens))
//	{
//		cur_token = (t_token *)ft_get_element(&env->tokens, i);
//		printf("%s", ft_substr(cur_token->start, 0, cur_token->size));
//	}
//	printf("\n");
	// debug end
}


