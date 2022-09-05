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
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <linux/limits.h>

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif 

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
	char	**values;
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
	t_vector	functions;
	char		**envp;
	bool		is_need_update_envp;
	char		*input_line;
	int			last_code;
}				t_environment;

typedef struct s_function
{
	char *name;
	int (*func)(t_environment *, const char *const*);
}		t_function;

void	ft_push(t_vector	*vector, const char	*string_var);
size_t	ft_find_by_name(const t_vector	*vector, const char	*name);
t_variable_env *ft_get_by_name(const t_vector	*vector, const char *name);

//		prompt.c
void	ft_set_new_prompt(const t_vector *variable_env, t_prompt	*prompt, t_info	info);

//		init.c
void	ft_init(t_environment	*env, char	**envp,
				const char	*name_shell);
//		error.c
void	ft_error(const char	*name_shell, const char	*err_msg);

//		commands_utilities.c
char	*ft_get_pwd(void);

//		commands.c
int		ft_command_cd(t_environment 	*env, const char *const *args);
int		ft_command_pwd(t_environment	*env, const char *const *args);
int		ft_command_env(t_environment	*env, const char *const *args);
int		ft_command_unset(t_environment	*env, const char *const *args);

//		commands_2.c
int		ft_command_exit(t_environment 	*env, const char *const *args);
int		ft_command_export(t_environment	*env, const char *const *args);
int		ft_command_echo(t_environment	*env, const char *const *args);

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_foreach(void **array, void (*func)(void *));
void	ft_smart_free(void	**address);
void	ft_smart_double_free(void	***address);
int		ft_smart_atoi(const char	*src, bool	*is_error);

//		signal.c
void	ft_initial_action(t_sigaction *action);

//		deinit.c
void	ft_deinit(t_environment	*env);

//		exit.c
int		ft_exit(t_environment	*env, int status, bool is_clean);

//		variable_env.c
bool	ft_convert_str_to_struct(t_variable_env	*dst, const char	*src);
bool	ft_convert_vector_to_array(char	***dst,	const t_vector	*src);

//		file_utilities.c
bool	ft_is_regular_file(char const *path);
bool	ft_is_exist(char const *path);
bool	ft_which(const char *const *paths, const char *name, char **dst);

//		print
void	ft_print_error(t_environment	*env, const char *command, const char *msg);
void	ft_print_errno(t_environment	*env, const char *command);

//		main.c
int		main(int argc, char **argv, char    **envp);

#endif
