#include "minishell.h"

int	parse_symbols(t_environment *env, char **cs, int *i, int *err)
{
	if ((*cs)[(*i)] == '\'')
	{
		(*cs) = str_qoutes((*cs), i, err);
		if ((*err))
			return (0);
	}
	if ((*cs)[(*i)] == '\"')
	{
		(*cs) = str_doub_qoutes((*cs), i, env, err);
		if ((*err))
			return (0);
	}
	if ((*cs)[(*i)] == '\\')
		(*cs) = str_slash((*cs), i);
	if ((*cs)[(*i)] == '$')
		(*cs) = str_expanding((*cs), i, env);
	else
		(*i)++;
	return (1);
}

void	go_throw_string(char **cs, int *i, t_environment *env, int *r)
{
	while ((*cs)[++(*i)])
	{
		if ((*cs)[*i] == '\\' && ((*cs)[*i + 1] == '\\'
				|| (*cs)[*i + 1] == '$' || (*cs)[*i + 1] == '\"'))
			(*cs) = str_slash((*cs), i);
		else if ((*cs)[*i] == '$')
			(*cs) = str_expanding((*cs), i, env);
		else if ((*cs)[*i] == '\"')
		{
			*r = 0;
			break ;
		}
	}
}

void	get_result_string(char **res, char *fp, char *lp, char *val)
{
	if (!val && !ft_strlen(fp) && !ft_strlen(lp))
	{
		free(fp);
		free(lp);
		*res = ft_strdup("");
	}
	else
	{
		*res = ft_strjoin_with_free(fp, val, 1, 1);
		*res = ft_strjoin_with_free(*res, lp, 1, 1);
	}
}
