#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <fcntl.h>

# include "get_next_line.h"

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

enum e_Token
{
	t_word,
	t_pipe,
	t_r_in,
	t_hd,
	t_r_out,
	t_r_outa
};

typedef struct s_token
{
	char	*start;
	int		size;
	int		type;
}	t_token;

typedef struct s_redir
{
	int		r_type;
	t_token	*arg;
}	t_redir;

typedef struct s_environment
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
	t_vector	tokens;
	t_vector	groups;
}				t_environment;

<<<<<<< HEAD
typedef struct s_command
{
	t_token		*command;
	t_vector	args;
	t_vector	redirs;
	int			(*builtin)(t_environment *, char *args);
}				t_command;
=======
typedef struct s_function
{
	char *name;
	int (*func)(t_environment *, const char *);
}		t_function;
>>>>>>> dev_illarion

void	ft_push(t_vector	*vector, const char	*string_var);
size_t	ft_find_by_name(const t_vector	*vector, const char	*name);

//		prompt.c
void	ft_set_new_prompt(t_prompt	*prompt, t_info	info);

//		init.c
<<<<<<< HEAD
void	ft_init(t_environment	*env, const char	**envp,
			const char	*name_shell);
=======
void	ft_init(t_environment	*env, char	**envp,
				const char	*name_shell);
>>>>>>> dev_illarion
//		error.c
void	ft_error(const char	*name_shell, const char	*err_msg);

//		commands.c
char	*ft_get_pwd(void);
<<<<<<< HEAD
int		ft_command_cd(t_environment *env, const char	*arg);
int		ft_command_pwd(t_environment *env, const char	*arg);
int		ft_command_env(t_environment *env, const char	*arg);
int		ft_command_unset(t_environment *env, const char	*arg);
=======
int		ft_command_cd(t_environment 	*env, const char	*arg);
int		ft_command_pwd(t_environment	*env, const char	*arg);
int		ft_command_env(t_environment	*env, const char	*arg);
int		ft_command_unset(t_environment	*env, const char	*arg);
>>>>>>> dev_illarion

//		commands_2.c
int		ft_command_exit(t_environment *env, const char	*arg);
int		ft_command_export(t_environment	*env, const char	*arg);
int		ft_command_echo(t_environment	*env, const char	*arg);

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_smart_free(void	**address);
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
<<<<<<< HEAD
bool	ft_convert_token_vector_to_str_array(char	***dst,	const t_vector	*src);

//		in_out_files.c
void	input_file_fd(t_redir *token);
void	output_file_fd(t_redir *token);

//		ft_isspace.c
int		ft_isspace(int c);

//		ft_errors_managment.c
void	cmd_not_found(char *const *in_argv);
int		ft_raise_error(char *strarg);
int		ft_raise_perror(char *strarg, int free_arg);
void	exit_find_failure(char **in_argv, char *access_denied_path);

//		get_next_line.c
char	*get_next_line(int fd);

//		here_doc.c
void	here_doc(t_redir *token, int pipe_fd[2]);

//		lexer.c
int		preparse(t_environment *env);
void	lexer(t_environment *env);

//		parser_utilities.c
void	expand_word(t_environment *env, char **start, int *size);

//		parser.c
void	parser(t_environment *env);

//		executor_utilities.c
void	find_cmd_in_path(char **args, char **envp);

//		executor.c
int		executor(t_environment *env);
=======

//		file_utilities.c
bool	ft_is_regular_file(char const *path);
bool	ft_is_exist(char const *path);
>>>>>>> dev_illarion

//		main.c
int		main(int argc, char **argv, char **envp);

#endif
