#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../lib/libft/libft.h"
# include "../lib/libvector/includes/vector.h"

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <errno.h>

typedef struct sigaction	t_sigaction;

typedef struct s_prompt
{
	char	*current_prompt;
	bool	is_need_change;
}	t_prompt;

typedef struct s_info
{
	char	*name_shell;
	char	*pwd;
}	t_info;

typedef struct s_variable_env
{
	char	*name;
	char	*value;
}			t_variable_env;

enum e_status
{
	SUCCESS = 0,
	COMMON_ERROR
};

typedef	struct s_environment
{
	t_prompt	prompt;
	t_info		info;
	t_sigaction	action;
	t_vector	variables_env;
	char		*input_line;
	int			last_code;
}				t_environment;

void	ft_push(t_vector	*vector, const char	*string_var);
size_t	ft_find_by_name(const t_vector	*vector, const char	*name);

//		prompt.c
void	ft_set_new_prompt(t_prompt	*prompt, t_info	info);

//		init.c
void	ft_init(t_environment	*env, const char	**envp,
				const char	*name_shell);
//		error.c
void	ft_error(const char	*name_shell, const char	*err_msg);

//		commands.c
char	*ft_get_pwd(void);
int		ft_command_cd(t_environment 	*env, const char	*arg);
int		ft_command_pwd(t_environment	*env);
int		ft_command_env(const t_environment	*env);
int		ft_command_unset(t_environment	*env, const char	*arg);

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_smart_free(void	**address);
int		ft_smart_atoi(const char	*src, bool	*is_error);

//		signal.c
void	ft_initial_action(t_sigaction *action);

//		main.c
int		main(int argc, char **argv, char    **envp);

#endif
