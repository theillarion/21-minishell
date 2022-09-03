#include "minishell.h"

char	*str_slash(char *current_string, int *i)
{
	char	*result;
	char	*firstpart;
	char	*lastpart;

	firstpart = ft_substr(current_string, 0, *i);
	lastpart = ft_strdup(current_string + *i);
	result = ft_strjoin_with_free(firstpart, lastpart, 1, 1);
	return (result);
}

char	*str_qoutes(char *current_str, int *i)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	while (current_str[++*i])
	{
		if (current_str[*i] == '\'')
			break ;
	}
	firstpart = ft_substr(current_str, 0, begin);
	in_qoutes_part = ft_substr(current_str, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(current_str + *i + 1);
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	return (result);
}

char	*str_expanding(char *current_string, int *i, t_environment *env)
{
	int				beginning;
	char			*result;
	char			*firstpart;
	char			*lastpart;
	char			*var_value;

	beginning = *i;
	while (current_string[++(*i)])
	{
		if (!ft_isalnum(current_string[*i]) && current_string[*i] != '_')
			break ;
	}
	firstpart = ft_substr(current_string, 0, beginning);
	lastpart = ft_strdup(current_string + *i);
	var_value = get_v(current_string, i, env, beginning);
	result = ft_strjoin_with_free(firstpart, var_value, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	return (result);
}

char	*str_double_qoutes(char *current_str, int *i, t_environment *env)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	while (current_str[++(*i)])
	{
		if (current_str[*i] == '\\' && (current_str[*i + 1] == '\\'
				|| current_str[*i + 1] == '$' || current_str[*i + 1] == '\"'))
			current_str = str_slash(current_str, i);
		if (current_str[*i] == '$')
			current_str = str_expanding(current_str, i, env);
		if (current_str[*i] == '\"')
			break ;
	}
	firstpart = ft_substr(current_str, 0, begin);
	in_qoutes_part = ft_substr(current_str, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(current_str + *i + 1);
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
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
