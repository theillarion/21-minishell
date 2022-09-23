/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 23:10:17 by illarion          #+#    #+#             */
/*   Updated: 2022/09/23 23:15:35 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif 

# define COUNT_FUNCTIONS 7

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

typedef struct s_environment
{
	t_vector	variables_env;
	t_vector	functions;
	t_sigaction	action;
	t_info		info;
	t_prompt	prompt;
	char		*input_line;
	char		**envp;
	bool		is_need_update_envp;
	int			last_code;
}				t_environment;

typedef struct s_function
{
	char	*name;
	int		(*func)(t_environment *, const char *const*);
}			t_function;

bool			ft_push(t_vector	*vector, const char	*string_var);
size_t			ft_find_by_name(const t_vector	*vector, const char	*name);
t_variable_env	*ft_get_by_name(const t_vector	*vector, const char *name);

//				prompt.c
void			ft_set_new_prompt(const t_vector *variable_env,
					t_prompt	*prompt, t_info	info);

//				fill.c
bool			ft_fill(t_environment	*env, char	**envp,
					const char	*name_shell);

//				init.c
void			ft_init(t_environment	*env);

//				commands_utilities.c
char			*ft_get_pwd(void);

//				commands.c
int				ft_command_cd(t_environment	*env, const char *const *args);
int				ft_command_pwd(t_environment	*env, const char *const *args);
int				ft_command_env(t_environment	*env, const char *const *args);
int				ft_command_unset(t_environment	*env, const char *const *args);

//				commands_2.c
int				ft_command_exit(t_environment	*env, const char *const *args);
int				ft_command_export(t_environment	*env, const char *const *args);
int				ft_command_echo(t_environment	*env, const char *const *args);

//				utilities.c
void			ft_foreach(void **array, void (*func)(void *));
void			ft_smart_free(void	**address);
void			ft_smart_double_free(void	***address);
int				ft_smart_atoi(const char	*src, bool	*is_error);
size_t			ft_size_array(void	**address);

//				signal.c
void			ft_handle_signal(int signal);
void			ft_handle_signal_child(int signal);

//				destroy.c
void			ft_destroy(t_environment	*env);

//				exit.c
int				ft_exit(t_environment	*env, int status);
void			ft_exit_with_message(t_environment	*env, int status,
					const char	*command, const char	*msg);

//				utilities_variable_env.c
char			*ft_get_str(const t_variable_env	*var_env);
bool			ft_convert_vector_to_array(char	***dst,	const t_vector	*src);

//				utilities_variable_env_2.c
bool			ft_convert_str_to_struct(const t_vector	*variable_env,
					t_variable_env	*dst, const char	*src);

//				file_utilities.c
bool			ft_is_regular_file(char const *path);
bool			ft_is_exist(char const *path);
bool			ft_which(const char *const *paths, const char *name,
					char **dst);

//				print
void			ft_print_error(const t_environment	*env, const char *command,
					const char *msg);
void			ft_print_errno(t_environment	*env, const char *command);

//				main.c
int				main(int argc, char	**argv, char	**envp);

#endif
