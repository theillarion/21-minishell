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

char	*str_expanding(char *current_s, int *i, t_environment *env)
{
	int				beginning;
	char			*result;
	char			*firstpart;
	char			*lastpart;
	char			*var_value;

	beginning = *i;
	while (current_s[++(*i)])
	{
		if (*i == beginning + 1 && current_s[*i] == '?' && (*i)++)
			break ;
		if (*i == beginning + 1 && !ft_isalpha(current_s[*i]) && (*i)++)
			break ;
		if (!ft_isalnum(current_s[*i]) && current_s[*i] != '_')
			break ;
	}
	firstpart = ft_substr(current_s, 0, beginning);
	lastpart = ft_strdup(current_s + *i);
	if (current_s[(*i) - 1] == '?')
		var_value = ft_itoa(env->last_code);
	else
		var_value = get_v(current_s, i, env, beginning);
	get_result_string(&result, firstpart, lastpart, var_value);
	free(current_s);
	return (result);
}

char	*str_doub_qoutes(char *cs, int *i, t_environment *env, int *r)
{
	int		begin;
	char	*firstpart;
	char	*in_qoutes_part;
	char	*lastpart;
	char	*result;

	begin = *i;
	*r = 2;
	go_throw_string(&cs, i, env, r);
	firstpart = ft_substr(cs, 0, begin);
	in_qoutes_part = ft_substr(cs, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(cs + *i + 1);
	result = ft_strjoin_with_free(firstpart, in_qoutes_part, 1, 1);
	result = ft_strjoin_with_free(result, lastpart, 1, 1);
	free(cs);
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
	i = 0;
	err = 0;
	while (current_string && current_string[i])
	{
		if (parse_symbols(env, &current_string, &i, &err) == 0)
			break ;
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