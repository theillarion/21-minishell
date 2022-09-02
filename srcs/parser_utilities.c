#include "minishell.h"

char	*str_slash(char *current_string, int *i)
{
	char	*result;
	char	*firstpart;
	char	*lastpart;

	firstpart = ft_substr(current_string, 0, *i);
	lastpart = ft_strdup(current_string + *i);
	result = ft_strjoin(firstpart, lastpart);
	free(firstpart);
	free(lastpart);
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
	result = ft_strjoin(firstpart, in_qoutes_part);
	free(firstpart);
	free(in_qoutes_part);
	firstpart = result;
	result = ft_strjoin(result, lastpart);
	free(firstpart);
	free(lastpart);
	return (result);
}

char	*str_expanding(char *current_string, int *i, t_environment *env)
{
	int				beginning;
	char			*result;
	char			*firstpart;
	char			*var_name;
	size_t			var_index;
	char			**var_value;
	char			*lastpart;
	t_variable_env	*var_element;

	beginning = *i;
	while (current_string[++(*i)])
	{
		if (!ft_isalnum(current_string[*i]) && current_string[*i] != '_')
			break ;
	}
	firstpart = ft_substr(current_string, 0, beginning);
	var_name = ft_substr(current_string, beginning + 1, *i - beginning - 1);
	var_index = ft_find_by_name(&env->variables_env, var_name);
	var_element = ft_get_element(&env->variables_env, var_index);
	var_value = var_element->values;
	lastpart = ft_strdup(current_string + *i);
	if (ft_strlen(var_name) > ft_strlen(*var_value))
		*i = *i - (int)(ft_strlen(var_name) - ft_strlen(*var_value));
	result = ft_strjoin(firstpart, *var_value);
	result = ft_strjoin(result, lastpart);
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
		if (current_str[*i] == '\\' && (current_str[*i + 1] == '\\' || current_str[*i + 1] == '$' || current_str[*i + 1] == '\"'))
			current_str = str_slash(current_str, i);
		if (current_str[*i] == '$')
			current_str = str_expanding(current_str, i, env);
		if (current_str[*i] == '\"')
			break ;
	}
	firstpart = ft_substr(current_str, 0, begin);
	in_qoutes_part = ft_substr(current_str, begin + 1, (*i - begin - 1));
	lastpart = ft_strdup(current_str + *i + 1);
	result = ft_strjoin(firstpart, in_qoutes_part);
	result = ft_strjoin(result, lastpart);
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
