#include "minishell.h"

char	*str_slash(char *current_string, int *i)
{
	char	*result;
	char	*firstpart;
	char	*lastpart;

	firstpart = ft_substr(current_string, 0, *i);
	lastpart = ft_strdup(current_string + *i);
	result = ft_strjoin_with_free(firstpart, lastpart, 1, 1);
	free(current_string);
	return (result);
}

char	*str_qoutes(char *current_str, int *i, int *r)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	*r = 1;
	while (current_str[++*i])
	{
		if (current_str[*i] == '\'')
		{
			*r = 0;
			break ;
		}
	}
	firstpart = ft_substr(current_str, 0, begin);
	in_qoutes_part = ft_substr(current_str, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(current_str + *i + 1);
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(current_str);
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
	if (!var_value && !ft_strlen(firstpart) && !ft_strlen(lastpart))
	{
		free(firstpart);
		free(lastpart);
		return (NULL);
	}
	result = ft_strjoin_with_free(firstpart, var_value, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(current_string);
	return (result);
}

char	*str_double_qoutes(char *current_s, int *i, t_environment *env, int *r)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	*r = 2;
	while (current_s[++(*i)])
	{
		if (current_s[*i] == '\\' && (current_s[*i + 1] == '\\'
				|| current_s[*i + 1] == '$' || current_s[*i + 1] == '\"'))
			current_s = str_slash(current_s, i);
		if (current_s[*i] == '$')
			current_s = str_expanding(current_s, i, env);
		if (current_s[*i] == '\"')
		{
			*r = 0;
			break ;
		}
	}
	firstpart = ft_substr(current_s, 0, begin);
	in_qoutes_part = ft_substr(current_s, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(current_s + *i + 1);
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(current_s);
	return (result);
}

int	expand_word(t_environment *env, char **start, int *size)
{
	char	*current_string;
	int		i;
	int		err;

	current_string = ft_substr(*start, 0, *size);
	if (!current_string)
		return (1);
	i = -1;
	err = 0;
	while (current_string && current_string[++i])
	{
		if (current_string[i] == '\'')
		{
			current_string = str_qoutes(current_string, &i, &err);
			if (err)
				break ;
		}
		if (current_string[i] == '\\')
			current_string = str_slash(current_string, &i);
		if (current_string[i] == '\"')
		{
			current_string = str_double_qoutes(current_string, &i, env, &err);
			if (err)
				break ;
		}
		if (current_string[i] == '$')
			current_string = str_expanding(current_string, &i, env);
	}
	*start = current_string;
	if (current_string)
		*size = (int)ft_strlen(current_string);
	else
		*size = 0;
	if (err)
		return (ft_syntax_error(env));
	return (err);
}
