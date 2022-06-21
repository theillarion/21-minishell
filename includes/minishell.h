#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../lib/libft/libft.h"

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


typedef	struct s_environment
{
	t_prompt	prompt;
	t_info		info;
}		t_environment;

//		prompt.c
void	ft_set_new_prompt(t_prompt	*prompt, t_info	info);

//		init.c
void	ft_init(t_environment	*env, const char	*name_shell);

//		error.c
void	ft_error(const char	*name_shell, const char	*err_msg);

//		commands.c
void	ft_command_cd(t_environment 	*env, const char	*arg);
char	*ft_get_pwd(void);
void	ft_commands_pwd(t_environment	*env);

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_smart_free(void	**address);

//		signal.c
void	ft_initial_action(t_sigaction *action);

//		main.c
int		main(int argc, char **argv, char    **envp);

#endif
